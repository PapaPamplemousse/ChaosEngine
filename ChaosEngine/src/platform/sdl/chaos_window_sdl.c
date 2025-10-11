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
 * @file chaos_window_sdl.c
 * @brief SDL2 backend for ChaosEngine window utilities.
 */
#include "platform/chaos_window.h"

#include "core/chaos_log.h"

#include <stdio.h>

ce_result ce_window_create(const ce_window_config* config, ce_window* out_window)
{
    ce_result result;
    ce_u32    window_flags;
    ce_u32    renderer_flags;

    result = CE_OK;
    window_flags = SDL_WINDOW_SHOWN;
    renderer_flags = SDL_RENDERER_ACCELERATED;

    if ((config == (const ce_window_config*)0) || (out_window == (ce_window*)0))
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "ce_window_create invalid arguments");
        result = (ce_result)(-((ce_s32)CHAOS_ERR_INVALID_ARGUMENT));
    }
    else
    {
        if (config->resizable == CE_TRUE)
        {
            window_flags |= SDL_WINDOW_RESIZABLE;
        }

        if (config->vsync == CE_TRUE)
        {
            renderer_flags |= SDL_RENDERER_PRESENTVSYNC;
        }

        out_window->native_window = SDL_CreateWindow(config->title != (const ce_char*)0 ? config->title : "ChaosEngine",
                                                     SDL_WINDOWPOS_CENTERED,
                                                     SDL_WINDOWPOS_CENTERED,
                                                     config->width,
                                                     config->height,
                                                     window_flags);
        if (out_window->native_window == (SDL_Window*)0)
        {
            CE_RAISE_ERROR(CHAOS_ERR_WINDOW_CREATION_FAILED, SDL_GetError());
            result = (ce_result)(-((ce_s32)CHAOS_ERR_WINDOW_CREATION_FAILED));
        }
        else
        {
            out_window->native_renderer = SDL_CreateRenderer(out_window->native_window, -1, renderer_flags);
            if (out_window->native_renderer == (SDL_Renderer*)0)
            {
                CE_RAISE_ERROR(CHAOS_ERR_GFX_INIT_FAILED, SDL_GetError());
                SDL_DestroyWindow(out_window->native_window);
                out_window->native_window = (SDL_Window*)0;
                result = (ce_result)(-((ce_s32)CHAOS_ERR_GFX_INIT_FAILED));
            }
            else
            {
                out_window->config = *config;
                ce_window_set_clear_color(out_window, ce_color_rgb(16u, 16u, 32u));
                CE_LOG_INFO("Window created: %dx%d", config->width, config->height);
            }
        }
    }

    return result;
}

void ce_window_destroy(ce_window* window)
{
    if (window == (ce_window*)0)
    {
        return;
    }

    if (window->native_renderer != (SDL_Renderer*)0)
    {
        SDL_DestroyRenderer(window->native_renderer);
        window->native_renderer = (SDL_Renderer*)0;
    }

    if (window->native_window != (SDL_Window*)0)
    {
        SDL_DestroyWindow(window->native_window);
        window->native_window = (SDL_Window*)0;
    }
}

void ce_window_set_clear_color(ce_window* window, ce_color color)
{
    if (window == (ce_window*)0)
    {
        return;
    }

    window->clear_color = color;
}

void ce_window_clear(ce_window* window)
{
    if ((window == (ce_window*)0) || (window->native_renderer == (SDL_Renderer*)0))
    {
        return;
    }

    SDL_SetRenderDrawColor(window->native_renderer,
                           window->clear_color.r,
                           window->clear_color.g,
                           window->clear_color.b,
                           window->clear_color.a);
    SDL_RenderClear(window->native_renderer);
}

void ce_window_present(ce_window* window)
{
    if ((window == (ce_window*)0) || (window->native_renderer == (SDL_Renderer*)0))
    {
        return;
    }

    SDL_RenderPresent(window->native_renderer);
}

void ce_window_draw_filled_rect(ce_window* window, ce_s32 x, ce_s32 y, ce_s32 w, ce_s32 h, ce_color color)
{
    SDL_Rect rect;

    if ((window == (ce_window*)0) || (window->native_renderer == (SDL_Renderer*)0))
    {
        return;
    }

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(window->native_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(window->native_renderer, &rect);
}

void ce_window_draw_rect(ce_window* window, ce_s32 x, ce_s32 y, ce_s32 w, ce_s32 h, ce_color color)
{
    SDL_Rect rect;

    if ((window == (ce_window*)0) || (window->native_renderer == (SDL_Renderer*)0))
    {
        return;
    }

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(window->native_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(window->native_renderer, &rect);
}

SDL_Renderer* ce_window_renderer(ce_window* window)
{
    if (window == (ce_window*)0)
    {
        return (SDL_Renderer*)0;
    }

    return window->native_renderer;
}
