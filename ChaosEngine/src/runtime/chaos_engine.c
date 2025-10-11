/**
 *
 * ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░ ░▒▓███████▓▒  ▒▓████████▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓████████▓▒░
 * ▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
 * ▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
 * ▒▓█▓▒░      ░▒▓████████▓▒░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░  ▒▓██████▓▒░ ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓██████▓▒░
 * ▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒  ▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
 * ▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒  ▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░
 * ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓███████▓▒░  ▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░
 *
 * @file chaos_engine.c
 * @brief Implementation of the ChaosEngine runtime loop.
 */
#include "runtime/chaos_engine.h"

#include <SDL2/SDL.h>

#include "utility/chaos_string.h"

/* ************************************************************************** */
/* INTERNAL CONSTANTS                                                         */
/* ************************************************************************** */

#define CE_DEFAULT_WIDTH  1280
#define CE_DEFAULT_HEIGHT 720
#define CE_DEFAULT_FPS    60u
#define CE_MAX_DT_CLAMP   0.25f

/* ************************************************************************** */
/* INTERNAL HELPERS                                                           */
/* ************************************************************************** */

static void ce__engine_reset(ce_engine* engine)
{
    if (engine == (ce_engine*)0)
    {
        return;
    }

    ce__memset(engine, 0u, sizeof(*engine));
}

static ce_result ce__engine_init_sdl(void)
{
    ce_result result;

    result = CE_OK;

    if (SDL_WasInit(0) == 0)
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0)
        {
            CE_RAISE_ERROR(CHAOS_ERR_PLATFORM_INIT_FAILED, SDL_GetError());
            result = (ce_result)(-((ce_s32)CHAOS_ERR_PLATFORM_INIT_FAILED));
        }
    }

    return result;
}

static void ce__engine_shutdown_sdl(void)
{
    SDL_Quit();
}

static void ce__engine_apply_config_defaults(ce_engine_config* config)
{
    if (config == (ce_engine_config*)0)
    {
        return;
    }

    if (config->window.width <= 0)
    {
        config->window.width = CE_DEFAULT_WIDTH;
    }

    if (config->window.height <= 0)
    {
        config->window.height = CE_DEFAULT_HEIGHT;
    }

    if (config->window.title == (const ce_char*)0)
    {
        config->window.title = "ChaosEngine";
    }

    if (config->target_fps == 0u)
    {
        config->target_fps = CE_DEFAULT_FPS;
    }

    if (config->max_updates_per_frame == 0u)
    {
        config->max_updates_per_frame = 5u;
    }
}

static void ce__engine_sleep_for_fps(ce_u32 target_fps, ce_f32 frame_time)
{
    ce_f32 target_dt;

    if (target_fps == 0u)
    {
        return;
    }

    target_dt = 1.0f / (ce_f32)target_fps;
    if (frame_time < target_dt)
    {
        ce_f32 remaining = target_dt - frame_time;
        if (remaining > 0.0f)
        {
            SDL_Delay((ce_u32)(remaining * 1000.0f));
        }
    }
}

/* ************************************************************************** */
/* PUBLIC API                                                                 */
/* ************************************************************************** */

ce_engine_config ce_engine_config_default(void)
{
    ce_engine_config cfg;

    cfg.window.title     = "ChaosEngine";
    cfg.window.width     = CE_DEFAULT_WIDTH;
    cfg.window.height    = CE_DEFAULT_HEIGHT;
    cfg.window.resizable = CE_TRUE;
    cfg.window.vsync     = CE_TRUE;
    cfg.target_fps       = CE_DEFAULT_FPS;
    cfg.max_updates_per_frame = 5u;

    return cfg;
}

ce_result ce_engine_run(const ce_engine_config* config, const ce_engine_callbacks* callbacks, void* user_data)
{
    ce_engine        engine;
    ce_engine_config cfg;
    ce_result        result;
    ce_bool          initialized;
    ce_bool          running;

    if ((config == (const ce_engine_config*)0) || (callbacks == (const ce_engine_callbacks*)0))
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "ce_engine_run invalid args");
        return (ce_result)(-((ce_s32)CHAOS_ERR_INVALID_ARGUMENT));
    }

    if (callbacks->on_update == (ce_engine_update_fn)0)
    {
        CE_RAISE_ERROR(CHAOS_ERR_ENGINE_LOOP_FAILED, "Update callback is mandatory");
        return (ce_result)(-((ce_s32)CHAOS_ERR_ENGINE_LOOP_FAILED));
    }

    cfg = *config;
    ce__engine_apply_config_defaults(&cfg);

    ce__engine_reset(&engine);
    engine.config    = cfg;
    engine.callbacks = *callbacks;
    engine.user_data = user_data;

    result = ce__engine_init_sdl();
    if (CE_FAILED(result))
    {
        return result;
    }

    initialized = CE_TRUE;

    if (CE_SUCCEEDED(result))
    {
        ce_error_global();
        ce_time_init();
        ce_input_init(&engine.input);

        result = ce_window_create(&engine.config.window, &engine.window);
        if (CE_FAILED(result))
        {
            initialized = CE_FALSE;
        }
    }

    if ((initialized == CE_TRUE) && (engine.callbacks.on_init != (ce_engine_init_fn)0))
    {
        if (engine.callbacks.on_init(&engine, user_data) == CE_FALSE)
        {
            CE_RAISE_ERROR(CHAOS_ERR_ENGINE_NOT_INITIALIZED, "Engine init callback failed");
            initialized = CE_FALSE;
            result = (ce_result)(-((ce_s32)CHAOS_ERR_ENGINE_NOT_INITIALIZED));
        }
    }

    if (initialized == CE_FALSE)
    {
        ce_window_destroy(&engine.window);
        ce_time_shutdown();
        ce__engine_shutdown_sdl();
        return result;
    }

    engine.running     = CE_TRUE;
    engine.fixed_dt    = 1.0f / (ce_f32)engine.config.target_fps;
    engine.accumulator = 0.0f;
    engine.elapsed_time = 0.0f;
    engine.frame_index  = 0u;

    running = CE_TRUE;

    while (running == CE_TRUE)
    {
        ce_f32 frame_dt;
        ce_u32 updates_performed;
        ce_f32 alpha;
        ce_f32 frame_start_delta;
        Uint64 frame_start_ticks;
        SDL_Event evt;

        frame_start_ticks = SDL_GetPerformanceCounter();

        ce_input_new_frame(&engine.input);

        while (SDL_PollEvent(&evt) == 1)
        {
            if (evt.type == SDL_QUIT)
            {
                engine.running = CE_FALSE;
            }
            ce_input_process_event(&engine.input, &evt);
        }

        ce_time_update();
        frame_dt = ce_time_get_delta();
        if (frame_dt > CE_MAX_DT_CLAMP)
        {
            frame_dt = CE_MAX_DT_CLAMP;
        }

        engine.accumulator += frame_dt;
        engine.elapsed_time += frame_dt;
        updates_performed = 0u;

        while ((engine.accumulator >= engine.fixed_dt) && (updates_performed < engine.config.max_updates_per_frame))
        {
            engine.callbacks.on_update(&engine, engine.user_data, engine.fixed_dt);
            engine.accumulator -= engine.fixed_dt;
            updates_performed++;
        }

        alpha = (engine.fixed_dt > 0.0f) ? (engine.accumulator / engine.fixed_dt) : 0.0f;

        if (engine.callbacks.on_render != (ce_engine_render_fn)0)
        {
            ce_window_clear(&engine.window);
            engine.callbacks.on_render(&engine, engine.user_data, alpha);
            ce_window_present(&engine.window);
        }

        engine.frame_index++;

        frame_start_delta = (ce_f32)(SDL_GetPerformanceCounter() - frame_start_ticks) /
                            (ce_f32)SDL_GetPerformanceFrequency();

        if (engine.config.window.vsync == CE_FALSE)
        {
            ce__engine_sleep_for_fps(engine.config.target_fps, frame_start_delta);
        }

        running = engine.running;
    }

    if (engine.callbacks.on_shutdown != (ce_engine_shutdown_fn)0)
    {
        engine.callbacks.on_shutdown(&engine, engine.user_data);
    }

    ce_window_destroy(&engine.window);
    ce_time_shutdown();
    ce__engine_shutdown_sdl();

    return result;
}

void ce_engine_request_exit(ce_engine* engine)
{
    if (engine == (ce_engine*)0)
    {
        return;
    }

    engine->running = CE_FALSE;
}

const ce_input_state* ce_engine_input(const ce_engine* engine)
{
    if (engine == (const ce_engine*)0)
    {
        return (const ce_input_state*)0;
    }

    return &engine->input;
}

ce_window* ce_engine_window(ce_engine* engine)
{
    if (engine == (ce_engine*)0)
    {
        return (ce_window*)0;
    }

    return &engine->window;
}

ce_f32 ce_engine_elapsed_time(const ce_engine* engine)
{
    if (engine == (const ce_engine*)0)
    {
        return 0.0f;
    }

    return engine->elapsed_time;
}

ce_u64 ce_engine_frame_index(const ce_engine* engine)
{
    if (engine == (const ce_engine*)0)
    {
        return 0u;
    }

    return engine->frame_index;
}
