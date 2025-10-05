/**
 * @file demo.c
 * @brief Rain & splash particle simulation — business logic only.
 *
 * This demo spawns falling raindrops, collides them with the ground (Y=0),
 * and spawns secondary splash particles. Rendering is fully delegated to the
 * ChaosRenderer via meshes, shaders, and the points stream API.
 *
 * - Uses CE_GetDeltaTime() for time step (provided by ChaosEngine), or you
 *   may replace it with a local SDL-based delta time if not available.
 * - All OpenGL calls are wrapped by the renderer; the demo never calls GL.
 *
 * Controls:
 *  - ESC: quit
 *  - R  : reset simulation
 *
 * @author
 *  ChaosEngine Team
 * @date
 *  October 2025
 * @version
 *  2.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "chaos_engine.h"  

/* -------------------------------------------------------------------------- */
/* Configuration                                                              */
/* -------------------------------------------------------------------------- */

#define MAX_DROPS               2048
#define DROPS_PER_SECOND        200.0f
#define DROP_START_HEIGHT       10.0f
#define GRAVITY_ACCELERATION    9.81f

#define SPLASH_PARTICLES_COUNT  12
#define SPLASH_LIFETIME         0.80f
#define SPLASH_SPREAD_FORCE     6.5f

#define DROP_RADIUS_MIN         0.03f
#define DROP_RADIUS_MAX         0.06f

#define SPAWN_AREA_HALF_EXTENT  15.0f
#define SPLASH_UP_BIAS          0.35f
#define SPLASH_DRAG             0.10f

#define GROUND_EXTENT           50.0f

#define MAX_SPLASHES            (MAX_DROPS * SPLASH_PARTICLES_COUNT)

/* -------------------------------------------------------------------------- */
/* Simulation types                                                           */
/* -------------------------------------------------------------------------- */

typedef struct {
    int   active;
    float x, y, z;
    float vx, vy, vz;
    float radius;
} RainDrop;

typedef struct {
    int   active;
    float x, y, z;
    float vx, vy, vz;
    float age, lifetime;
} SplashParticle;

/* GPU payload for chaos_points_* API (must match its layout) */
typedef struct {
    float x, y, z;
    float size;
    float alpha;
} GPUParticle;

/* -------------------------------------------------------------------------- */
/* Globals: simulation & rendering handles                                    */
/* -------------------------------------------------------------------------- */

static chaos_renderer_t gRenderer;

/* Particles */
static RainDrop        gDrops[MAX_DROPS];
static SplashParticle  gSplashes[MAX_SPLASHES];

/* Counts (for quick stats) */
static int gActiveDrops = 0;
static int gActiveSplashes = 0;

/* Spawn accumulator for rate-based spawn */
static float gSpawnAcc = 0.0f;

/* Ground resources */
static chaos_mesh_t     gGround;
static chaos_material_t gGroundMat;

/* Points streams and shaders */
static chaos_points_t gPS_Drops;
static chaos_points_t gPS_Splashes;

static chaos_shader_t gSh_Particles;   /* shared shader for drops & splashes */

/* Uniform locations (cached) */
static t_sint32 u_P_PointScale = -1;
static t_sint32 u_P_Color      = -1;

/* CPU staging buffers */
static GPUParticle* gStageDrops = NULL;
static GPUParticle* gStageSpl   = NULL;

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static float frand01(void) { return (float)rand() / (float)RAND_MAX; }
static float frand_range(float a, float b) { return a + (b - a) * frand01(); }
static float clamp01(float x) { return x < 0.f ? 0.f : (x > 1.f ? 1.f : x); }

/* -------------------------------------------------------------------------- */
/* Particle shader sources (expects binding=0 UBO for view/proj)              */
/* -------------------------------------------------------------------------- */

static const char* VS_PARTICLES =
    "#version 450 core\n"
    "layout(std140, binding=0) uniform UBO { mat4 uView; mat4 uProj; };\n"
    "layout(location=0) in vec3 aPos;\n"
    "layout(location=1) in float aSize;\n"
    "layout(location=2) in float aAlpha;\n"
    "uniform float uPointScale;\n"
    "out float vAlpha;\n"
    "void main(){\n"
    "  vec4 viewPos = uView * vec4(aPos,1.0);\n"
    "  gl_Position = uProj * viewPos;\n"
    "  float dist = max(0.001, -viewPos.z);\n"
    "  gl_PointSize = max(1.0, (uPointScale * aSize) / dist);\n"
    "  vAlpha = aAlpha;\n"
    "}\n";

static const char* FS_PARTICLES =
    "#version 450 core\n"
    "layout(location=0) out vec4 oColor;\n"
    "in float vAlpha;\n"
    "uniform vec4 uColor;\n"
    "void main(){\n"
    "  vec2 p = gl_PointCoord * 2.0 - 1.0;\n"
    "  float d2 = dot(p,p);\n"
    "  if (d2 > 1.0) discard;\n"
    "  float soft = 1.0 - smoothstep(0.9, 1.0, sqrt(d2));\n"
    "  oColor = vec4(uColor.rgb, uColor.a * soft * vAlpha);\n"
    "}\n";

/* -------------------------------------------------------------------------- */
/* Simulation                                                                 */
/* -------------------------------------------------------------------------- */

static void ResetSimulation(void)
{
    memset(gDrops,    0, sizeof gDrops);
    memset(gSplashes, 0, sizeof gSplashes);
    gActiveDrops = gActiveSplashes = 0;
    gSpawnAcc = 0.0f;
}

static int FindFreeDrop(void) {
    for (int i=0;i<MAX_DROPS;++i) if (!gDrops[i].active) return i;
    return -1;
}

static int FindFreeSplash(void) {
    for (int i=0;i<MAX_SPLASHES;++i) if (!gSplashes[i].active) return i;
    return -1;
}

static void SpawnDrop(void)
{
    const int idx = FindFreeDrop();
    if (idx < 0) return;

    gDrops[idx].active = 1;
    gDrops[idx].x = frand_range(-SPAWN_AREA_HALF_EXTENT, SPAWN_AREA_HALF_EXTENT);
    gDrops[idx].y = DROP_START_HEIGHT;
    gDrops[idx].z = frand_range(-SPAWN_AREA_HALF_EXTENT, SPAWN_AREA_HALF_EXTENT);
    gDrops[idx].vx = gDrops[idx].vz = gDrops[idx].vy = 0.0f;
    gDrops[idx].radius = frand_range(DROP_RADIUS_MIN, DROP_RADIUS_MAX);
    ++gActiveDrops;
}

static void SpawnSplashBurst(float px, float pz, float baseRadius)
{
    (void)baseRadius; /* not used now; keep for possible scaling */
    for (int k=0; k<SPLASH_PARTICLES_COUNT; ++k) {
        const int id = FindFreeSplash();
        if (id < 0) break;
        const float ang   = frand_range(0.0f, (float)(2.0 * C_PI));
        const float speed = SPLASH_SPREAD_FORCE * frand_range(0.5f, 1.0f);
        const float ux = chaos_cos(ang);
        const float uz = chaos_sin(ang);
        const float vy = SPLASH_UP_BIAS * speed * frand_range(0.6f, 1.2f);

        gSplashes[id].active = 1;
        gSplashes[id].x = px;  gSplashes[id].y = 0.01f; gSplashes[id].z = pz;
        gSplashes[id].vx = ux * speed; gSplashes[id].vy = vy; gSplashes[id].vz = uz * speed;
        gSplashes[id].age = 0.0f;
        gSplashes[id].lifetime = SPLASH_LIFETIME * frand_range(0.8f, 1.2f);
        ++gActiveSplashes;
    }
}

static void UpdateSimulation(void)
{
    /* Use ChaosEngine time step if available */
    float dt = chaos_time_get_delta();

    if (dt <= 0.0f) dt = 1.0f/60.0f;

    /* Spawn */
    gSpawnAcc += dt * DROPS_PER_SECOND;
    for (int i=0, n=(int)gSpawnAcc; i<n; ++i) SpawnDrop();
    gSpawnAcc -= (float)((int)gSpawnAcc);

    const float gy = -GRAVITY_ACCELERATION;

    /* Drops */
    for (int i=0;i<MAX_DROPS;++i) {
        if (!gDrops[i].active) continue;
        gDrops[i].vy += gy * dt;
        gDrops[i].x  += gDrops[i].vx * dt;
        gDrops[i].y  += gDrops[i].vy * dt;
        gDrops[i].z  += gDrops[i].vz * dt;

        if (gDrops[i].y - gDrops[i].radius <= 0.0f) {
            SpawnSplashBurst(gDrops[i].x, gDrops[i].z, gDrops[i].radius);
            gDrops[i].active = 0; --gActiveDrops;
        }
    }

    /* Splashes */
    for (int i=0;i<MAX_SPLASHES;++i) {
        if (!gSplashes[i].active) continue;
        gSplashes[i].age += dt;
        if (gSplashes[i].age >= gSplashes[i].lifetime) {
            gSplashes[i].active = 0; --gActiveSplashes; continue;
        }
        gSplashes[i].vy += gy * dt;
        gSplashes[i].x  += gSplashes[i].vx * dt;
        gSplashes[i].y  += gSplashes[i].vy * dt;
        gSplashes[i].z  += gSplashes[i].vz * dt;

        gSplashes[i].vx *= (1.0f - SPLASH_DRAG * dt);
        gSplashes[i].vy *= (1.0f - SPLASH_DRAG * dt);
        gSplashes[i].vz *= (1.0f - SPLASH_DRAG * dt);

        if (gSplashes[i].y <= 0.0f && gSplashes[i].vy < 0.0f) {
            gSplashes[i].y  = 0.0f;
            gSplashes[i].vy = -gSplashes[i].vy * 0.25f;
            gSplashes[i].age += 0.15f * dt;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Rendering (high-level calls to the renderer only)                          */
/* -------------------------------------------------------------------------- */

static void CreateGround(void)
{
    const float E = GROUND_EXTENT;
    const t_float32 pos[] = {
        -E,0,-E,  E,0,-E,  E,0,E,  -E,0,E
    };
    const t_uint32 idx[] = { 0,1,2, 0,2,3 };
    gGround = chaos_mesh_create(pos, NULL, NULL, 4, idx, 6);

    gGroundMat.type = CHAOS_MAT_UNLIT_COLOR;
    gGroundMat.tex  = NULL;
    /* Slightly reflective wet asphalt look */
    gGroundMat.color[0] = 0.12f;  
    gGroundMat.color[1] = 0.14f;
    gGroundMat.color[2] = 0.18f;
    gGroundMat.color[3] = 1.0f;

}

static void DestroyGround(void)
{
    chaos_mesh_destroy(&gGround);
}

/* Build particle rendering resources (points streams + shader) */
static void CreateParticlesRender(void)
{
    gPS_Drops    = chaos_points_create(MAX_DROPS);
    gPS_Splashes = chaos_points_create(MAX_SPLASHES);

    gSh_Particles = chaos_shader_create(VS_PARTICLES, FS_PARTICLES);
    u_P_PointScale = chaos_shader_uniform(&gSh_Particles, "uPointScale");
    u_P_Color      = chaos_shader_uniform(&gSh_Particles, "uColor");

    gStageDrops = (GPUParticle*)malloc(MAX_DROPS     * sizeof(GPUParticle));
    gStageSpl   = (GPUParticle*)malloc(MAX_SPLASHES  * sizeof(GPUParticle));
}

static void DestroyParticlesRender(void)
{
    free(gStageDrops); gStageDrops = NULL;
    free(gStageSpl);   gStageSpl   = NULL;
    chaos_points_destroy(&gPS_Drops);
    chaos_points_destroy(&gPS_Splashes);
    chaos_shader_destroy(&gSh_Particles);
}

static void RenderSimulation(void)
{
    chaos_begin_frame(&gRenderer, 0.04f, 0.05f, 0.06f, 1.0f);

    /* Ground */
    mat4 M = m4_identity();
        GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    printf("[DEBUG] Active program: %d\n", currentProgram);
    chaos_draw_mesh(&gRenderer, &gGround, &gGroundMat, &M);

    /* Build staging arrays */
    int nd=0, ns=0;

    for (int i=0;i<MAX_DROPS;++i) if (gDrops[i].active) {
        gStageDrops[nd].x = gDrops[i].x;
        gStageDrops[nd].y = gDrops[i].y;
        gStageDrops[nd].z = gDrops[i].z;
        gStageDrops[nd].size  = gDrops[i].radius;
        gStageDrops[nd].alpha = 1.0f;
        ++nd;
    }

    for (int i=0;i<MAX_SPLASHES;++i) if (gSplashes[i].active) {
        gStageSpl[ns].x = gSplashes[i].x;
        gStageSpl[ns].y = gSplashes[i].y;
        gStageSpl[ns].z = gSplashes[i].z;
        gStageSpl[ns].size  = DROP_RADIUS_MIN * 0.75f;
        const float t = clamp01(gSplashes[i].age / (gSplashes[i].lifetime + 1e-6f));
        gStageSpl[ns].alpha = 1.0f - t;
        ++ns;
    }

    /* Upload to GPU */
    chaos_points_update(&gPS_Drops,    gStageDrops, nd, sizeof(GPUParticle));
    chaos_points_update(&gPS_Splashes, gStageSpl,   ns, sizeof(GPUParticle));

    /* Configure shader uniforms once */
    glUseProgram(gSh_Particles._gl_prog); /* allowed: through renderer shader handle */

    const float pointScale = chaos_compute_point_scale(&gRenderer);
    chaos_shader_set_f32(u_P_PointScale, pointScale);

    /* Draw drops (deep blue) */
    {
        const t_float32 c[4] = {0.25f, 0.45f, 1.0f, 0.9f};
        chaos_shader_set_vec4(u_P_Color, c);
        chaos_points_draw(&gPS_Drops, &gSh_Particles, (t_uint32)nd);
    }

    /* Draw splashes (lighter blue) */
    {
        const t_float32 c[4] = {0.60f, 0.80f, 1.0f, 0.7f};
        chaos_shader_set_vec4(u_P_Color, c);
        chaos_points_draw(&gPS_Splashes, &gSh_Particles, (t_uint32)ns);
    }

    chaos_end_frame(&gRenderer);
}

/**
 * @brief Cleanly shuts down the demo and all ChaosEngine subsystems.
 */
static void chaos_demo_quit(void)
{
    printf("[ChaosDemo] Shutting down...\n");

    DestroyParticlesRender();
    DestroyGround();

    chaos_renderer_shutdown(&gRenderer);
    chaos_time_shutdown();

    printf("[ChaosDemo] Clean exit.\n");
}


/* -------------------------------------------------------------------------- */
/* Main                                                                       */
/* -------------------------------------------------------------------------- */

int main(void)
{
    srand((unsigned)time(NULL));

    if (chaos_renderer_init(&gRenderer, "ChaosEngine - Rain Particles", 1280, 720,
                            CHAOS_BACKEND_OPENGL45, 0) != 0)
    {
        fprintf(stderr, "[ChaosDemo] Failed to initialize ChaosRenderer\n");
        return EXIT_FAILURE;
    }

    chaos_time_init();

    /* Camera setup (isometric view) */
    vec_t eye    = (vec_t){ .x = 15.0f, .y = 10.0f, .z = 15.0f };
    vec_t target = (vec_t){ .x = 0.0f,  .y = 0.0f,  .z = 0.0f };
    vec_t up     = (vec_t){ .x = 0.0f,  .y = 1.0f,  .z = 0.0f };
    chaos_camera_setup(&gRenderer, eye, target, up, 55.0f, 0.1f, 300.0f);

    /* Build render resources */
    CreateGround();
    CreateParticlesRender();

    /* Simulation */
    ResetSimulation();

    t_bool running = TRUE;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = FALSE;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = FALSE;
                        break;

                    case SDLK_r:
                        ResetSimulation();
                        break;

                    default:
                        break;
                }
            }
            else if (e.type == SDL_WINDOWEVENT &&
                     e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                chaos_renderer_resize(&gRenderer, e.window.data1, e.window.data2);
            }
        }

        /* If ESC was pressed, exit cleanly */
        if (!running)
            break;

        chaos_time_update();
        UpdateSimulation();
        RenderSimulation();
    }

    chaos_demo_quit();
    return EXIT_SUCCESS;
}