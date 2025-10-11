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
 * @file chaos_time.c
 * @brief Implementation skeleton.
 */
#include "core/chaos_time.h"
#include "core/chaos_error.h"
#include <SDL2/SDL.h>

/* ************************************************************************** */
/* INTERNAL STATE                                                             */
/* ************************************************************************** */

/** @brief SDL performance counter at previous frame. */
static ce_u64 g_time_last = 0;
/** @brief SDL performance counter at initialization. */
static ce_u64 g_time_start = 0;
/** @brief Seconds elapsed between last two frames. */
static ce_f32 g_time_delta = 0.0f;
/** @brief SDL performance counter frequency (ticks per second). */
static ce_f64 g_time_freq = 0.0;
/** @brief True if chaos_time_init() has been called. */
static ce_bool g_time_initialized = CE_FALSE;

/* ************************************************************************** */
/* PUBLIC FUNCTIONS                                                           */
/* ************************************************************************** */


void ce_time_init(void)
{
    ce_s32 sdl_ret = 0;

    if (g_time_initialized == CE_TRUE)
    {
        /* Already initialized → warning, not fatal */
        CE_RAISE_WARN(CHAOS_WARN_UNOPTIMIZED_PATH, "ce_time_init called twice");
    }
    else
    {
        sdl_ret = SDL_InitSubSystem(SDL_INIT_TIMER);
        if (sdl_ret != 0)
        {
            CE_RAISE_ERROR(CHAOS_ERR_PLATFORM_INIT_FAILED, "SDL timer subsystem init failed");
        }
        else
        {
            g_time_start = SDL_GetPerformanceCounter();
            g_time_last  = g_time_start;
            g_time_freq  = (ce_f64)SDL_GetPerformanceFrequency();
            g_time_delta = 0.0F;
            g_time_initialized = CE_TRUE;        }
    }
}


void ce_time_update(void)
{
    ce_result result = CE_OK;
    ce_u64 now = 0U;
    ce_f64 diff = 0.0;

    if (g_time_initialized == CE_FALSE)
    {
        CE_RAISE_WARN(CHAOS_WARN_UNINITIALIZED_VALUE, "ce_time_update called before initialization");
        result = -CHAOS_ERR_ENGINE_NOT_INITIALIZED;
    }
    else
    {
        now = SDL_GetPerformanceCounter();
        diff = (ce_f64)(now - g_time_last);
        g_time_delta = (ce_f32)(diff / g_time_freq);
        g_time_last = now;
        result = CE_OK;
    }

    (void)result; /* prevent unused warning */
}

ce_f32 ce_time_get_delta(void)
{
    return g_time_delta;
}

ce_f32 ce_time_get_since_start(void)
{
    if (!g_time_initialized) return 0.0f;
    ce_u64 now = SDL_GetPerformanceCounter();
    return (ce_f32)((ce_f64)(now - g_time_start) / g_time_freq);
}

void ce_time_shutdown(void)
{
    if (!g_time_initialized) return;
    SDL_QuitSubSystem(SDL_INIT_TIMER);
    g_time_initialized = CE_FALSE;
}

