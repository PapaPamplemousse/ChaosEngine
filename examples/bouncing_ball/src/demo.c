// /**
//  * @file demo.c
//  * @brief Simple bouncing ball demo with orbit camera controls.
//  *
//  * Keyboard Controls:
//  *  - Arrows : orbit camera around scene
//  *  - + / -  : zoom in / out
//  *  - R      : reset camera
//  *  - ESC    : quit
//  *
//  * The demo shows a white sphere bouncing on a green ground plane,
//  * using ChaosEngine’s physics, renderer, and time subsystems.
//  */

// #include <SDL2/SDL.h>
// #include "chaos_engine.h"

// /* -------------------------------------------------------------------------- */
// /* Simulation state                                                           */
// /* -------------------------------------------------------------------------- */

// typedef struct {
//     t_float32 x, y, z;
//     t_float32 vy;
// } BallState;

// static BallState gBall = { 0.0f, 5.0f, 0.0f, 0.0f };
// static t_float32 G = -9.81f;
// static t_float32 BOUNCE = 0.75f;
// static t_bool gRunning = TRUE;

// /* Rendering */
// static chaos_renderer_t gRenderer;
// static chaos_mesh_t     gBallMesh;
// static chaos_material_t gBallMat;
// static chaos_mesh_t     gGroundMesh;
// static chaos_material_t gGroundMat;

// /* -------------------------------------------------------------------------- */
// /* Input handler                                                              */
// /* -------------------------------------------------------------------------- */

// static void handle_input(chaos_renderer_t* r)
// {
//     SDL_Event e;
//     while (SDL_PollEvent(&e))
//     {
//         switch (e.type)
//         {
//         case SDL_QUIT:
//             gRunning = FALSE;
//             break;

//         case SDL_KEYDOWN:
//             switch (e.key.keysym.sym)
//             {
//             case SDLK_ESCAPE:
//                 gRunning = FALSE;
//                 break;
//             case SDLK_UP:
//                 chaos_camera_orbit(r, 0.0f, -10.0f);
//                 break;
//             case SDLK_DOWN:
//                 chaos_camera_orbit(r, 0.0f, +10.0f);
//                 break;
//             case SDLK_LEFT:
//                 chaos_camera_orbit(r, -10.0f, 0.0f);
//                 break;
//             case SDLK_RIGHT:
//                 chaos_camera_orbit(r, +10.0f, 0.0f);
//                 break;
//             case SDLK_PLUS:
//             case SDLK_KP_PLUS:
//                 chaos_camera_zoom(r, -1.0f);
//                 break;
//             case SDLK_MINUS:
//             case SDLK_KP_MINUS:
//                 chaos_camera_zoom(r, +1.0f);
//                 break;
//             case SDLK_r:
//                 chaos_camera_reset(r);
//                 break;
//             default:
//                 break;
//             }
//             break;
//         }
//     }
// }

// /* -------------------------------------------------------------------------- */
// /* Simulation update                                                          */
// /* -------------------------------------------------------------------------- */

// static void update_simulation(void)
// {
//     t_float32 dt = chaos_time_get_delta();
//     if (dt <= 0.0f) dt = 1.0f / 60.0f;

//     gBall.vy += G * dt;
//     gBall.y  += gBall.vy * dt;

//     if (gBall.y < 0.5f)
//     {
//         gBall.y = 0.5f;
//         gBall.vy = -gBall.vy * BOUNCE;

//         if (chaos_abs(gBall.vy) < 0.5f)
//             gBall.vy = 0.0f;
//     }
// }

// /* -------------------------------------------------------------------------- */
// /* Rendering                                                                  */
// /* -------------------------------------------------------------------------- */

// static void render_scene(void)
// {
//     chaos_begin_frame(&gRenderer, 0.1f, 0.25f, 0.1f, 1.0f); // greenish background

//     mat4 M_ball = m4_translate(vec3(gBall.x, gBall.y, gBall.z));
//     chaos_draw_mesh(&gRenderer, &gBallMesh, &gBallMat, &M_ball);

//     mat4 M_ground = m4_identity();
//     chaos_draw_mesh(&gRenderer, &gGroundMesh, &gGroundMat, &M_ground);

//     chaos_end_frame(&gRenderer);
// }

// /* -------------------------------------------------------------------------- */
// /* Scene setup / teardown                                                     */
// /* -------------------------------------------------------------------------- */

// static void create_scene(void)
// {
//     /* Create a simple sphere (you must have chaos_mesh_create_sphere implemented) */
//     gBallMesh = chaos_mesh_create_sphere(0.5f, 24, 16);
//     gBallMat.type = CHAOS_MAT_UNLIT_COLOR;
//     gBallMat.tex = NULL;
//     gBallMat.color[0] = 1.0f;
//     gBallMat.color[1] = 1.0f;
//     gBallMat.color[2] = 1.0f;
//     gBallMat.color[3] = 1.0f;

//     /* Ground plane */
//     const t_float32 E = 20.0f;
//     const t_float32 pos[] = { -E,0,-E,  E,0,-E,  E,0,E,  -E,0,E };
//     const t_uint32 idx[]  = { 0,1,2, 0,2,3 };
//     gGroundMesh = chaos_mesh_create(pos, NULL, NULL, 4, idx, 6);

//     gGroundMat.type = CHAOS_MAT_UNLIT_COLOR;
//     gGroundMat.tex = NULL;
//     gGroundMat.color[0] = 0.1f;
//     gGroundMat.color[1] = 0.8f;
//     gGroundMat.color[2] = 0.1f;
//     gGroundMat.color[3] = 1.0f;
// }

// static void destroy_scene(void)
// {
//     chaos_mesh_destroy(&gBallMesh);
//     chaos_mesh_destroy(&gGroundMesh);
// }

// /* -------------------------------------------------------------------------- */
// /* Main                                                                       */
// /* -------------------------------------------------------------------------- */

// int main(void)
// {
//     if (chaos_renderer_init(&gRenderer, "ChaosEngine - Bouncing Ball", 1280, 720,
//                             CHAOS_BACKEND_OPENGL45, 0) != 0)
//     {
//         fprintf(stderr, "Failed to initialize renderer\n");
//         return 1;
//     }

//     chaos_camera_reset(&gRenderer);
//     create_scene();

//     while (gRunning)
//     {
//         handle_input(&gRenderer);
//         chaos_time_update();
//         update_simulation();
//         render_scene();
//     }

//     destroy_scene();
//     chaos_renderer_shutdown(&gRenderer);
//     return 0;
// }

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include "chaos_engine.h"

/* -------------------------------------------------------------------------- */
/* Ball physics                                                               */
/* -------------------------------------------------------------------------- */

typedef struct {
    float x, y;
    float vy;
} Ball;

static Ball ball = { .x = 640.0f, .y = 100.0f, .vy = 0.0f };
static const float GRAVITY   = 800.0f;
static const float BOUNCE    = 0.7f;
static const float FLOOR_Y   = 650.0f;
static const float BALL_SIZE = 64.0f;

static chaos_renderer_t renderer;
static t_bool running = TRUE;

/* -------------------------------------------------------------------------- */
/* Input handler                                                              */
/* -------------------------------------------------------------------------- */
static void handle_input(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = FALSE;
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                running = FALSE;
                break;
            case SDLK_r:
                ball.y = 100.0f;
                ball.vy = 0.0f;
                break;
            default: break;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Simulation                                                                 */
/* -------------------------------------------------------------------------- */
static void update_simulation(void)
{
    float dt = chaos_time_get_delta();
    if (dt <= 0.0f) dt = 1.0f / 60.0f;

    ball.vy += GRAVITY * dt;
    ball.y  += ball.vy * dt;

    if (ball.y + BALL_SIZE > FLOOR_Y) {
        ball.y = FLOOR_Y - BALL_SIZE;
        ball.vy = -ball.vy * BOUNCE;
        if (fabs(ball.vy) < 40.0f)
            ball.vy = 0.0f;
    }

    printf("Frame: ball.y=%.2f vy=%.2f\n", ball.y, ball.vy);
}

/* -------------------------------------------------------------------------- */
/* Rendering (2D helper with solid color)                                     */
/* -------------------------------------------------------------------------- */
static void render_sprite_2D_color(chaos_renderer_t* r, float x, float y, float w, float h, float rgba[4])
{
    /* Interleaved vertex data: pos(3), normal(3), uv(2) */
    float verts[] = {
         0, 0, 0,   0,0,1,   0,0,
         w, 0, 0,   0,0,1,   1,0,
         w, h, 0,   0,0,1,   1,1,
         0, h, 0,   0,0,1,   0,1
    };
    unsigned int idx[] = { 0,1,2, 0,2,3 };

    chaos_mesh_t quad = chaos_mesh_create(verts, verts+3, verts+6, 4, idx, 6);

    /* Simple orthographic projection */
    mat4 V = m4_identity();
    mat4 P = m4_identity();
    P.m[0]  =  2.0f / (float)r->width;
    P.m[5]  = -2.0f / (float)r->height;
    P.m[10] = -1.0f;
    P.m[12] = -1.0f;
    P.m[13] =  1.0f;

    r->camera.view = V;
    r->camera.proj = P;
    chaos_upload_vp_ubo(r);

    chaos_material_t m = {0};
    m.type = CHAOS_MAT_UNLIT_COLOR;
    m.color[0] = rgba[0];
    m.color[1] = rgba[1];
    m.color[2] = rgba[2];
    m.color[3] = rgba[3];

    mat4 M = m4_identity();
    M.m[12] = x;
    M.m[13] = y;
    M.m[14] = -1.0f;

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        printf("⚠️ gl pre-draw error: 0x%x\n", err);

    chaos_draw_mesh(r, &quad, &m, &M);

    err = glGetError();
    if (err != GL_NO_ERROR)
        printf("⚠️ gl post-draw error: 0x%x\n", err);

    chaos_mesh_destroy(&quad);
}

/* -------------------------------------------------------------------------- */
/* Render loop                                                                */
/* -------------------------------------------------------------------------- */
static void render_scene(void)
{
    chaos_begin_frame(&renderer, 0.15f, 0.25f, 0.15f, 1.0f);

    float rgba_ball[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    render_sprite_2D_color(&renderer,
        ball.x - BALL_SIZE / 2.0f,
        ball.y - BALL_SIZE / 2.0f,
        BALL_SIZE, BALL_SIZE, rgba_ball);

    chaos_end_frame(&renderer);
}

/* -------------------------------------------------------------------------- */
/* Main                                                                       */
/* -------------------------------------------------------------------------- */
int main(void)
{
    if (chaos_renderer_init(&renderer, "ChaosEngine - 2D Ball Demo",
                            1280, 720, CHAOS_BACKEND_OPENGL45, 0) != 0)
    {
        fprintf(stderr, "❌ Renderer init failed\n");
        return 1;
    }

    chaos_time_update();

    while (running) {
        handle_input();
        chaos_time_update();
        update_simulation();
        render_scene();
    }

    chaos_renderer_shutdown(&renderer);
    return 0;
}
