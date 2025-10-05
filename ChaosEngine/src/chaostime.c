/**
 * @file chaostime.c
 * @brief Implementation of the ChaosEngine timing subsystem (SDL2 backend).
 */

#include "chaos_time.h"
#include <SDL2/SDL.h>

/* ************************************************************************** */
/* INTERNAL STATE                                                             */
/* ************************************************************************** */

/** @brief SDL performance counter at previous frame. */
static t_uint64 g_time_last = 0;
/** @brief SDL performance counter at initialization. */
static t_uint64 g_time_start = 0;
/** @brief Seconds elapsed between last two frames. */
static t_float32 g_time_delta = 0.0f;
/** @brief SDL performance counter frequency (ticks per second). */
static t_float64 g_time_freq = 0.0;
/** @brief True if chaos_time_init() has been called. */
static t_bool g_time_initialized = FALSE;

/* ************************************************************************** */
/* PUBLIC FUNCTIONS                                                           */
/* ************************************************************************** */

void chaos_time_init(void)
{
    if (SDL_InitSubSystem(SDL_INIT_TIMER) != 0)
    {
        SDL_Log("chaos_time_init: SDL timer init failed: %s", SDL_GetError());
        return;
    }

    g_time_start = SDL_GetPerformanceCounter();
    g_time_last  = g_time_start;
    g_time_freq  = (t_float64)SDL_GetPerformanceFrequency();
    g_time_delta = 0.0f;
    g_time_initialized = TRUE;
}

void chaos_time_update(void)
{
    if (!g_time_initialized) return;

    t_uint64 now = SDL_GetPerformanceCounter();
    g_time_delta = (t_float32)((t_float64)(now - g_time_last) / g_time_freq);
    g_time_last = now;
}

t_float32 chaos_time_get_delta(void)
{
    return g_time_delta;
}

t_float32 chaos_time_get_since_start(void)
{
    if (!g_time_initialized) return 0.0f;
    t_uint64 now = SDL_GetPerformanceCounter();
    return (t_float32)((t_float64)(now - g_time_start) / g_time_freq);
}

void chaos_time_shutdown(void)
{
    if (!g_time_initialized) return;
    SDL_QuitSubSystem(SDL_INIT_TIMER);
    g_time_initialized = FALSE;
}
