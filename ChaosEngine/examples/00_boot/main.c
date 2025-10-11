#include "runtime/chaos_engine.h"
#include "utility/chaos_math.h"
#include "utility/chaos_string.h"

typedef struct boot_state_s
{
    ce_f32 pos_x;
    ce_f32 pos_y;
    ce_f32 speed;
    ce_color background;
    ce_color player_color;
} boot_state;

static ce_bool boot_on_init(ce_engine* engine, void* user_data)
{
    boot_state* state = (boot_state*)user_data;
    ce_window* window = ce_engine_window(engine);

    if ((state == (boot_state*)0) || (window == (ce_window*)0))
    {
        return CE_FALSE;
    }

    state->speed = 240.0f;
    state->background = ce_color_rgb(12u, 10u, 30u);
    state->player_color = ce_color_rgb(230u, 120u, 70u);

    ce_window_set_clear_color(window, state->background);
    state->pos_x = (ce_f32)(engine->config.window.width / 2 - 32);
    state->pos_y = (ce_f32)(engine->config.window.height / 2 - 32);

    CE_LOG_INFO("ChaosEngine demo booted");
    return CE_TRUE;
}

static void boot_on_update(ce_engine* engine, void* user_data, ce_f32 dt)
{
    const ce_input_state* input = ce_engine_input(engine);
    boot_state*           state = (boot_state*)user_data;
    ce_f32                dir_x;
    ce_f32                dir_y;

    if ((state == (boot_state*)0) || (input == (const ce_input_state*)0))
    {
        return;
    }

    dir_x = 0.0f;
    dir_y = 0.0f;

    if (ce_input_key_down(input, CE_KEY_A) || ce_input_key_down(input, CE_KEY_LEFT))
    {
        dir_x -= 1.0f;
    }
    if (ce_input_key_down(input, CE_KEY_D) || ce_input_key_down(input, CE_KEY_RIGHT))
    {
        dir_x += 1.0f;
    }
    if (ce_input_key_down(input, CE_KEY_W) || ce_input_key_down(input, CE_KEY_UP))
    {
        dir_y -= 1.0f;
    }
    if (ce_input_key_down(input, CE_KEY_S) || ce_input_key_down(input, CE_KEY_DOWN))
    {
        dir_y += 1.0f;
    }

    if ((dir_x != 0.0f) || (dir_y != 0.0f))
    {
        ce_f32 length = ce_sqrt((dir_x * dir_x) + (dir_y * dir_y));
        if (length > 0.0f)
        {
            dir_x /= length;
            dir_y /= length;
        }
        state->pos_x += dir_x * state->speed * dt;
        state->pos_y += dir_y * state->speed * dt;
    }

    if (ce_input_key_pressed(input, CE_KEY_ESCAPE))
    {
        ce_engine_request_exit(engine);
    }
}

static void boot_on_render(ce_engine* engine, void* user_data, ce_f32 alpha)
{
    boot_state* state = (boot_state*)user_data;
    ce_window*  window = ce_engine_window(engine);
    ce_s32      draw_x;
    ce_s32      draw_y;

    (void)alpha;

    if ((state == (boot_state*)0) || (window == (ce_window*)0))
    {
        return;
    }

    draw_x = (ce_s32)state->pos_x;
    draw_y = (ce_s32)state->pos_y;

    ce_window_draw_filled_rect(window, draw_x, draw_y, 64, 64, state->player_color);
    ce_window_draw_rect(window, draw_x - 2, draw_y - 2, 68, 68, ce_color_rgb(255u, 255u, 255u));
}

static void boot_on_shutdown(ce_engine* engine, void* user_data)
{
    (void)engine;
    (void)user_data;
    CE_LOG_INFO("ChaosEngine demo shutdown");
}

int main(void)
{
    ce_engine_config     config;
    ce_engine_callbacks  callbacks;
    boot_state           state;

    config = ce_engine_config_default();
    config.window.title  = "ChaosEngine Boot Demo";
    config.window.vsync  = CE_TRUE;
    config.target_fps    = 60u;

    callbacks.on_init     = boot_on_init;
    callbacks.on_update   = boot_on_update;
    callbacks.on_render   = boot_on_render;
    callbacks.on_shutdown = boot_on_shutdown;

    ce__memset(&state, 0u, sizeof(state));

    return (ce_engine_run(&config, &callbacks, &state) == CE_OK) ? 0 : 1;
}
