/**
 * @file demo.c
 * @brief ChaosEngine example: Orbit camera + pan + zoom around an imported asset.
 */

#include "core/chaos_types.h"
#include "core/chaos_error.h"
#include "core/chaos_time.h"
#include "core/chaos_memory.h"
#include "utility/chaos_math.h"
#include "gfx/chaos_scene.h"
#include "resources/chaos_assets.h"
#include <SDL2/SDL.h>

#define WIN_W (1024)
#define WIN_H (720)
#define ARENA_BYTES (16 * 1024 * 1024)

int main(void)
{
    int           ret       = 0;
    SDL_Window*   win       = (SDL_Window*)0;
    SDL_Renderer* rnd       = (SDL_Renderer*)0;
    ce_u8*        arena_mem = (ce_u8*)0;
    ce_arena      arena;
    ce_mesh_t     mesh;
    ce_scene_t    scene;
    ce_bool       running = CE_TRUE;

    /* --- Camera control sensitivities --- */
    ce_f32 yaw_sens   = 0.004f;
    ce_f32 pitch_sens = 0.004f;
    ce_f32 zoom_sens  = 0.08f;
    ce_f32 pan_factor = 0.002f; /* multiplied by distance for screen→world scaling */

    /* --- Input state --- */
    ce_f32  last_x   = 0.0f;
    ce_f32  last_y   = 0.0f;
    ce_bool orbiting = CE_FALSE;
    ce_bool panning  = CE_FALSE;

    /* === MEMORY INIT ==================================================== */
    arena_mem = (ce_u8*)SDL_malloc(ARENA_BYTES);
    if (arena_mem == (ce_u8*)0)
    {
        CE_RAISE_FATAL(CHAOS_ERR_OUT_OF_MEMORY, "arena malloc failed");
        ret = -1;
    }
    else
    {
        ce_arena_init(&arena, arena_mem, (ce_size)ARENA_BYTES);

        /* === SDL INIT ==================================================== */
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
        {
            CE_RAISE_FATAL(CHAOS_ERR_PLATFORM_INIT_FAILED, "SDL init failed");
            ret = -1;
        }
        else
        {
            win = SDL_CreateWindow("ChaosEngine — Orbit + Pan + Zoom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
            rnd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if ((win == (SDL_Window*)0) || (rnd == (SDL_Renderer*)0))
            {
                CE_RAISE_FATAL(CHAOS_ERR_WINDOW_CREATION_FAILED, "Window or renderer creation failed");
                ret = -1;
            }
            else
            {
                /* === CHAOSENGINE INIT ==================================== */
                ce_time_init();
                ce_scene_init(&scene);

                /* Load a binary STL asset */
                if (ce_assets_load_stl_binary(&arena, "examples/01_orbit_asset/data/Ghost_Sheet.stl", &mesh) != CE_OK)
                {
                    CE_RAISE_ERROR(CHAOS_ERR_RESOURCE_LOAD_FAILED, "STL load failed");
                    ret = -1;
                }
                else
                {
                    /* Add mesh and fit camera automatically to bounds */
                    ce_scene_add_mesh(&scene, &mesh);
                    ce_camera_fit_bounds(&scene.cam, &mesh.bounds);

                    /* Main loop */
                    while (running == CE_TRUE)
                    {
                        SDL_Event ev;

                        /* === EVENT LOOP ==================================== */
                        while (SDL_PollEvent(&ev) != 0)
                        {
                            if (ev.type == SDL_QUIT)
                            {
                                running = CE_FALSE;
                            }
                            else if (ev.type == SDL_MOUSEBUTTONDOWN)
                            {
                                if (ev.button.button == SDL_BUTTON_LEFT)
                                {
                                    orbiting = CE_TRUE;
                                    last_x   = (ce_f32)ev.button.x;
                                    last_y   = (ce_f32)ev.button.y;
                                }
                                else if (ev.button.button == SDL_BUTTON_RIGHT)
                                {
                                    panning = CE_TRUE;
                                    last_x  = (ce_f32)ev.button.x;
                                    last_y  = (ce_f32)ev.button.y;
                                }
                            }
                            else if (ev.type == SDL_MOUSEBUTTONUP)
                            {
                                if (ev.button.button == SDL_BUTTON_LEFT)
                                {
                                    orbiting = CE_FALSE;
                                }
                                else if (ev.button.button == SDL_BUTTON_RIGHT)
                                {
                                    panning = CE_FALSE;
                                }
                            }
                            else if (ev.type == SDL_MOUSEMOTION)
                            {
                                ce_f32 x  = (ce_f32)ev.motion.x;
                                ce_f32 y  = (ce_f32)ev.motion.y;
                                ce_f32 dx = x - last_x;
                                ce_f32 dy = y - last_y;

                                if (orbiting == CE_TRUE)
                                {
                                    /* Rotate around target */
                                    ce_camera_orbit(&scene.cam, dx * yaw_sens, -dy * pitch_sens, 0.0f);
                                }
                                else if (panning == CE_TRUE)
                                {
                                    /* Translate target in view plane */
                                    ce_f32 pan_speed = scene.cam.distance * pan_factor;
                                    ce_camera_pan(&scene.cam, dx, dy, pan_speed);
                                }

                                last_x = x;
                                last_y = y;
                            }
                            else if (ev.type == SDL_MOUSEWHEEL)
                            {
                                /* Smooth zoom in/out */
                                ce_camera_orbit(&scene.cam, 0.0f, 0.0f, (ev.wheel.y > 0) ? -zoom_sens : zoom_sens);
                            }
                            else if (ev.type == SDL_KEYDOWN)
                            {
                                if (ev.key.keysym.sym == SDLK_ESCAPE)
                                {
                                    running = CE_FALSE;
                                }
                            }
                        }

                        /* === UPDATE FRAME ================================== */
                        ce_time_update();

                        /* Clear + render mesh in wireframe */
                        SDL_SetRenderDrawColor(rnd, 16, 20, 24, 255);
                        SDL_RenderClear(rnd);

                        SDL_SetRenderDrawColor(rnd, 0, 255, 128, 255);
                        ce_scene_render_software(&scene, rnd, (ce_u32)WIN_W, (ce_u32)WIN_H);

                        SDL_RenderPresent(rnd);
                    }
                }
            }
        }
    }

    /* === CLEANUP ========================================================= */
    if (rnd != (SDL_Renderer*)0)
    {
        SDL_DestroyRenderer(rnd);
    }
    if (win != (SDL_Window*)0)
    {
        SDL_DestroyWindow(win);
    }

    SDL_Quit();

    if (arena_mem != (ce_u8*)0)
    {
        SDL_free(arena_mem);
    }

    return ret;
}
