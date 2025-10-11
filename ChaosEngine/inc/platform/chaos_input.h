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
 * @file chaos_input.h
 * @brief Keyboard/mouse input API.
 * @author PapaPamplemousse
 */
#ifndef CHAOS_INPUT_H
#define CHAOS_INPUT_H

#include "core/chaos_types.h"

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Supported keyboard keys for ChaosEngine input.
 */
typedef enum ce_key_e
{
    CE_KEY_UNKNOWN = 0,
    CE_KEY_ESCAPE,
    CE_KEY_SPACE,
    CE_KEY_RETURN,
    CE_KEY_TAB,
    CE_KEY_BACKSPACE,
    CE_KEY_LEFT,
    CE_KEY_RIGHT,
    CE_KEY_UP,
    CE_KEY_DOWN,
    CE_KEY_A,
    CE_KEY_B,
    CE_KEY_C,
    CE_KEY_D,
    CE_KEY_E,
    CE_KEY_F,
    CE_KEY_G,
    CE_KEY_H,
    CE_KEY_I,
    CE_KEY_J,
    CE_KEY_K,
    CE_KEY_L,
    CE_KEY_M,
    CE_KEY_N,
    CE_KEY_O,
    CE_KEY_P,
    CE_KEY_Q,
    CE_KEY_R,
    CE_KEY_S,
    CE_KEY_T,
    CE_KEY_U,
    CE_KEY_V,
    CE_KEY_W,
    CE_KEY_X,
    CE_KEY_Y,
    CE_KEY_Z,
    CE_KEY_0,
    CE_KEY_1,
    CE_KEY_2,
    CE_KEY_3,
    CE_KEY_4,
    CE_KEY_5,
    CE_KEY_6,
    CE_KEY_7,
    CE_KEY_8,
    CE_KEY_9,
    CE_KEY_LSHIFT,
    CE_KEY_RSHIFT,
    CE_KEY_LCTRL,
    CE_KEY_RCTRL,
    CE_KEY_LALT,
    CE_KEY_RALT,
    CE_KEY_COUNT
} ce_key;

/**
 * @brief Mouse buttons tracked by the input subsystem.
 */
typedef enum ce_mouse_button_e
{
    CE_MOUSE_LEFT = 0,
    CE_MOUSE_RIGHT,
    CE_MOUSE_MIDDLE,
    CE_MOUSE_X1,
    CE_MOUSE_X2,
    CE_MOUSE_BUTTON_COUNT
} ce_mouse_button;

/**
 * @brief Aggregate state for keyboard and mouse per-frame.
 */
typedef struct ce_input_state_s
{
    ce_bool key_down[CE_KEY_COUNT];
    ce_bool key_pressed[CE_KEY_COUNT];
    ce_bool key_released[CE_KEY_COUNT];

    ce_bool mouse_down[CE_MOUSE_BUTTON_COUNT];
    ce_bool mouse_pressed[CE_MOUSE_BUTTON_COUNT];
    ce_bool mouse_released[CE_MOUSE_BUTTON_COUNT];

    ce_s32 mouse_x;
    ce_s32 mouse_y;
    ce_s32 mouse_delta_x;
    ce_s32 mouse_delta_y;

    ce_s32 scroll_x;
    ce_s32 scroll_y;
} ce_input_state;

/**
 * @brief Initializes an input state structure.
 */
void ce_input_init(ce_input_state* state);

/**
 * @brief Clears transient pressed/released flags before processing a frame.
 */
void ce_input_new_frame(ce_input_state* state);

/**
 * @brief Processes a key transition.
 */
void ce_input_handle_key(ce_input_state* state, ce_key key, ce_bool is_down);

/**
 * @brief Processes a mouse button transition.
 */
void ce_input_handle_mouse_button(ce_input_state* state, ce_mouse_button button, ce_bool is_down);

/**
 * @brief Updates mouse coordinates and delta.
 */
void ce_input_handle_mouse_motion(ce_input_state* state, ce_s32 x, ce_s32 y, ce_s32 dx, ce_s32 dy);

/**
 * @brief Updates scroll wheel data.
 */
void ce_input_handle_mouse_wheel(ce_input_state* state, ce_s32 dx, ce_s32 dy);

/**
 * @brief Convenience wrapper processing an SDL_Event.
 */
void ce_input_process_event(ce_input_state* state, const SDL_Event* event);

ce_bool ce_input_key_down(const ce_input_state* state, ce_key key);
ce_bool ce_input_key_pressed(const ce_input_state* state, ce_key key);
ce_bool ce_input_key_released(const ce_input_state* state, ce_key key);

ce_bool ce_input_mouse_down(const ce_input_state* state, ce_mouse_button button);
ce_bool ce_input_mouse_pressed(const ce_input_state* state, ce_mouse_button button);
ce_bool ce_input_mouse_released(const ce_input_state* state, ce_mouse_button button);

ce_s32 ce_input_mouse_x(const ce_input_state* state);
ce_s32 ce_input_mouse_y(const ce_input_state* state);
ce_s32 ce_input_mouse_delta_x(const ce_input_state* state);
ce_s32 ce_input_mouse_delta_y(const ce_input_state* state);
ce_s32 ce_input_scroll_x(const ce_input_state* state);
ce_s32 ce_input_scroll_y(const ce_input_state* state);

#ifdef __cplusplus
}
#endif

#endif /* CHAOS_INPUT_H */
