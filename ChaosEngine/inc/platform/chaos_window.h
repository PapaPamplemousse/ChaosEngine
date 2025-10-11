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
 * @file chaos_window.h
 * @brief Window creation and management.
 * @author PapaPamplemousse
 */
#ifndef CHAOS_WINDOW_H
#define CHAOS_WINDOW_H

#include "core/chaos_types.h"
#include "core/chaos_error.h"

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief RGBA color helper structure.
 */
typedef struct ce_color_s
{
    ce_u8 r;
    ce_u8 g;
    ce_u8 b;
    ce_u8 a;
} ce_color;

/**
 * @brief Utility constructor for ce_color.
 */
static inline ce_color ce_color_rgba(ce_u8 r, ce_u8 g, ce_u8 b, ce_u8 a)
{
    ce_color c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}

/**
 * @brief Utility constructor for opaque RGB colors.
 */
static inline ce_color ce_color_rgb(ce_u8 r, ce_u8 g, ce_u8 b)
{
    return ce_color_rgba(r, g, b, 255u);
}

/**
 * @brief Window creation description.
 */
typedef struct ce_window_config_s
{
    const ce_char* title;
    ce_s32         width;
    ce_s32         height;
    ce_bool        resizable;
    ce_bool        vsync;
} ce_window_config;

/**
 * @brief Runtime window state.
 */
typedef struct ce_window_s
{
    SDL_Window*   native_window;
    SDL_Renderer* native_renderer;
    ce_window_config config;
    ce_color         clear_color;
} ce_window;

/**
 * @brief Initializes a window using SDL2.
 */
ce_result ce_window_create(const ce_window_config* config, ce_window* out_window);

/**
 * @brief Destroys an existing window.
 */
void ce_window_destroy(ce_window* window);

/**
 * @brief Sets the clear color used when calling @ref ce_window_clear.
 */
void ce_window_set_clear_color(ce_window* window, ce_color color);

/**
 * @brief Clears the current back buffer with the configured clear color.
 */
void ce_window_clear(ce_window* window);

/**
 * @brief Presents the back buffer to the screen.
 */
void ce_window_present(ce_window* window);

/**
 * @brief Draws a filled rectangle using the SDL renderer backend.
 */
void ce_window_draw_filled_rect(ce_window* window, ce_s32 x, ce_s32 y, ce_s32 w, ce_s32 h, ce_color color);

/**
 * @brief Draws a rectangle outline using the SDL renderer backend.
 */
void ce_window_draw_rect(ce_window* window, ce_s32 x, ce_s32 y, ce_s32 w, ce_s32 h, ce_color color);

/**
 * @brief Returns the underlying SDL_Renderer pointer.
 */
SDL_Renderer* ce_window_renderer(ce_window* window);

#ifdef __cplusplus
}
#endif

#endif /* CHAOS_WINDOW_H */
