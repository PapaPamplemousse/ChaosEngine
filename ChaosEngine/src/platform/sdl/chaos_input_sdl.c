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
 * @file chaos_input_sdl.c
 * @brief SDL2-backed input processing for ChaosEngine.
 */
#include "platform/chaos_input.h"

#include "core/chaos_error.h"
#include "utility/chaos_string.h"

/* ************************************************************************** */
/* INTERNAL HELPERS                                                           */
/* ************************************************************************** */

static ce_key ce__from_scancode(SDL_Scancode scancode)
{
    switch (scancode)
    {
        case SDL_SCANCODE_ESCAPE: return CE_KEY_ESCAPE;
        case SDL_SCANCODE_SPACE:  return CE_KEY_SPACE;
        case SDL_SCANCODE_RETURN: return CE_KEY_RETURN;
        case SDL_SCANCODE_TAB:    return CE_KEY_TAB;
        case SDL_SCANCODE_BACKSPACE: return CE_KEY_BACKSPACE;
        case SDL_SCANCODE_LEFT:   return CE_KEY_LEFT;
        case SDL_SCANCODE_RIGHT:  return CE_KEY_RIGHT;
        case SDL_SCANCODE_UP:     return CE_KEY_UP;
        case SDL_SCANCODE_DOWN:   return CE_KEY_DOWN;
        case SDL_SCANCODE_A: return CE_KEY_A;
        case SDL_SCANCODE_B: return CE_KEY_B;
        case SDL_SCANCODE_C: return CE_KEY_C;
        case SDL_SCANCODE_D: return CE_KEY_D;
        case SDL_SCANCODE_E: return CE_KEY_E;
        case SDL_SCANCODE_F: return CE_KEY_F;
        case SDL_SCANCODE_G: return CE_KEY_G;
        case SDL_SCANCODE_H: return CE_KEY_H;
        case SDL_SCANCODE_I: return CE_KEY_I;
        case SDL_SCANCODE_J: return CE_KEY_J;
        case SDL_SCANCODE_K: return CE_KEY_K;
        case SDL_SCANCODE_L: return CE_KEY_L;
        case SDL_SCANCODE_M: return CE_KEY_M;
        case SDL_SCANCODE_N: return CE_KEY_N;
        case SDL_SCANCODE_O: return CE_KEY_O;
        case SDL_SCANCODE_P: return CE_KEY_P;
        case SDL_SCANCODE_Q: return CE_KEY_Q;
        case SDL_SCANCODE_R: return CE_KEY_R;
        case SDL_SCANCODE_S: return CE_KEY_S;
        case SDL_SCANCODE_T: return CE_KEY_T;
        case SDL_SCANCODE_U: return CE_KEY_U;
        case SDL_SCANCODE_V: return CE_KEY_V;
        case SDL_SCANCODE_W: return CE_KEY_W;
        case SDL_SCANCODE_X: return CE_KEY_X;
        case SDL_SCANCODE_Y: return CE_KEY_Y;
        case SDL_SCANCODE_Z: return CE_KEY_Z;
        case SDL_SCANCODE_0: return CE_KEY_0;
        case SDL_SCANCODE_1: return CE_KEY_1;
        case SDL_SCANCODE_2: return CE_KEY_2;
        case SDL_SCANCODE_3: return CE_KEY_3;
        case SDL_SCANCODE_4: return CE_KEY_4;
        case SDL_SCANCODE_5: return CE_KEY_5;
        case SDL_SCANCODE_6: return CE_KEY_6;
        case SDL_SCANCODE_7: return CE_KEY_7;
        case SDL_SCANCODE_8: return CE_KEY_8;
        case SDL_SCANCODE_9: return CE_KEY_9;
        case SDL_SCANCODE_LSHIFT: return CE_KEY_LSHIFT;
        case SDL_SCANCODE_RSHIFT: return CE_KEY_RSHIFT;
        case SDL_SCANCODE_LCTRL:  return CE_KEY_LCTRL;
        case SDL_SCANCODE_RCTRL:  return CE_KEY_RCTRL;
        case SDL_SCANCODE_LALT:   return CE_KEY_LALT;
        case SDL_SCANCODE_RALT:   return CE_KEY_RALT;
        default: return CE_KEY_UNKNOWN;
    }
}

static ce_mouse_button ce__from_mouse_button(ce_u8 sdl_button)
{
    switch (sdl_button)
    {
        case SDL_BUTTON_LEFT:   return CE_MOUSE_LEFT;
        case SDL_BUTTON_RIGHT:  return CE_MOUSE_RIGHT;
        case SDL_BUTTON_MIDDLE: return CE_MOUSE_MIDDLE;
        case SDL_BUTTON_X1:     return CE_MOUSE_X1;
        case SDL_BUTTON_X2:     return CE_MOUSE_X2;
        default:                return CE_MOUSE_LEFT;
    }
}

/* ************************************************************************** */
/* PUBLIC API                                                                 */
/* ************************************************************************** */

void ce_input_init(ce_input_state* state)
{
    if (state == (ce_input_state*)0)
    {
        return;
    }

    ce__memset(state, 0u, sizeof(*state));
}

void ce_input_new_frame(ce_input_state* state)
{
    ce_size i;

    if (state == (ce_input_state*)0)
    {
        return;
    }

    for (i = 0; i < (ce_size)CE_KEY_COUNT; ++i)
    {
        state->key_pressed[i]  = CE_FALSE;
        state->key_released[i] = CE_FALSE;
    }

    for (i = 0; i < (ce_size)CE_MOUSE_BUTTON_COUNT; ++i)
    {
        state->mouse_pressed[i]  = CE_FALSE;
        state->mouse_released[i] = CE_FALSE;
    }

    state->mouse_delta_x = 0;
    state->mouse_delta_y = 0;
    state->scroll_x      = 0;
    state->scroll_y      = 0;
}

void ce_input_handle_key(ce_input_state* state, ce_key key, ce_bool is_down)
{
    if ((state == (ce_input_state*)0) || (key <= CE_KEY_UNKNOWN) || (key >= CE_KEY_COUNT))
    {
        return;
    }

    if (is_down == CE_TRUE)
    {
        if (state->key_down[key] == CE_FALSE)
        {
            state->key_pressed[key] = CE_TRUE;
        }
        state->key_down[key] = CE_TRUE;
    }
    else
    {
        if (state->key_down[key] == CE_TRUE)
        {
            state->key_released[key] = CE_TRUE;
        }
        state->key_down[key] = CE_FALSE;
    }
}

void ce_input_handle_mouse_button(ce_input_state* state, ce_mouse_button button, ce_bool is_down)
{
    if ((state == (ce_input_state*)0) || (button >= CE_MOUSE_BUTTON_COUNT))
    {
        return;
    }

    if (is_down == CE_TRUE)
    {
        if (state->mouse_down[button] == CE_FALSE)
        {
            state->mouse_pressed[button] = CE_TRUE;
        }
        state->mouse_down[button] = CE_TRUE;
    }
    else
    {
        if (state->mouse_down[button] == CE_TRUE)
        {
            state->mouse_released[button] = CE_TRUE;
        }
        state->mouse_down[button] = CE_FALSE;
    }
}

void ce_input_handle_mouse_motion(ce_input_state* state, ce_s32 x, ce_s32 y, ce_s32 dx, ce_s32 dy)
{
    if (state == (ce_input_state*)0)
    {
        return;
    }

    state->mouse_x       = x;
    state->mouse_y       = y;
    state->mouse_delta_x += dx;
    state->mouse_delta_y += dy;
}

void ce_input_handle_mouse_wheel(ce_input_state* state, ce_s32 dx, ce_s32 dy)
{
    if (state == (ce_input_state*)0)
    {
        return;
    }

    state->scroll_x += dx;
    state->scroll_y += dy;
}

void ce_input_process_event(ce_input_state* state, const SDL_Event* event)
{
    ce_key          key;
    ce_mouse_button button;

    if ((state == (ce_input_state*)0) || (event == (const SDL_Event*)0))
    {
        return;
    }

    switch (event->type)
    {
        case SDL_KEYDOWN:
            if (event->key.repeat == 0)
            {
                key = ce__from_scancode(event->key.keysym.scancode);
                ce_input_handle_key(state, key, CE_TRUE);
            }
            break;
        case SDL_KEYUP:
            key = ce__from_scancode(event->key.keysym.scancode);
            ce_input_handle_key(state, key, CE_FALSE);
            break;
        case SDL_MOUSEBUTTONDOWN:
            button = ce__from_mouse_button(event->button.button);
            ce_input_handle_mouse_button(state, button, CE_TRUE);
            break;
        case SDL_MOUSEBUTTONUP:
            button = ce__from_mouse_button(event->button.button);
            ce_input_handle_mouse_button(state, button, CE_FALSE);
            break;
        case SDL_MOUSEMOTION:
            ce_input_handle_mouse_motion(state,
                                         event->motion.x,
                                         event->motion.y,
                                         event->motion.xrel,
                                         event->motion.yrel);
            break;
        case SDL_MOUSEWHEEL:
            ce_input_handle_mouse_wheel(state, event->wheel.x, event->wheel.y);
            break;
        default:
            break;
    }
}

ce_bool ce_input_key_down(const ce_input_state* state, ce_key key)
{
    if ((state == (const ce_input_state*)0) || (key <= CE_KEY_UNKNOWN) || (key >= CE_KEY_COUNT))
    {
        return CE_FALSE;
    }

    return state->key_down[key];
}

ce_bool ce_input_key_pressed(const ce_input_state* state, ce_key key)
{
    if ((state == (const ce_input_state*)0) || (key <= CE_KEY_UNKNOWN) || (key >= CE_KEY_COUNT))
    {
        return CE_FALSE;
    }

    return state->key_pressed[key];
}

ce_bool ce_input_key_released(const ce_input_state* state, ce_key key)
{
    if ((state == (const ce_input_state*)0) || (key <= CE_KEY_UNKNOWN) || (key >= CE_KEY_COUNT))
    {
        return CE_FALSE;
    }

    return state->key_released[key];
}

ce_bool ce_input_mouse_down(const ce_input_state* state, ce_mouse_button button)
{
    if ((state == (const ce_input_state*)0) || (button >= CE_MOUSE_BUTTON_COUNT))
    {
        return CE_FALSE;
    }

    return state->mouse_down[button];
}

ce_bool ce_input_mouse_pressed(const ce_input_state* state, ce_mouse_button button)
{
    if ((state == (const ce_input_state*)0) || (button >= CE_MOUSE_BUTTON_COUNT))
    {
        return CE_FALSE;
    }

    return state->mouse_pressed[button];
}

ce_bool ce_input_mouse_released(const ce_input_state* state, ce_mouse_button button)
{
    if ((state == (const ce_input_state*)0) || (button >= CE_MOUSE_BUTTON_COUNT))
    {
        return CE_FALSE;
    }

    return state->mouse_released[button];
}

ce_s32 ce_input_mouse_x(const ce_input_state* state)
{
    if (state == (const ce_input_state*)0)
    {
        return 0;
    }
    return state->mouse_x;
}

ce_s32 ce_input_mouse_y(const ce_input_state* state)
{
    if (state == (const ce_input_state*)0)
    {
        return 0;
    }
    return state->mouse_y;
}

ce_s32 ce_input_mouse_delta_x(const ce_input_state* state)
{
    if (state == (const ce_input_state*)0)
    {
        return 0;
    }
    return state->mouse_delta_x;
}

ce_s32 ce_input_mouse_delta_y(const ce_input_state* state)
{
    if (state == (const ce_input_state*)0)
    {
        return 0;
    }
    return state->mouse_delta_y;
}

ce_s32 ce_input_scroll_x(const ce_input_state* state)
{
    if (state == (const ce_input_state*)0)
    {
        return 0;
    }
    return state->scroll_x;
}

ce_s32 ce_input_scroll_y(const ce_input_state* state)
{
    if (state == (const ce_input_state*)0)
    {
        return 0;
    }
    return state->scroll_y;
}
