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
 * @file chaos_engine.h
 * @brief Engine init/shutdown and main loop.
 * @author PapaPamplemousse
 */
#ifndef CHAOS_ENGINE_H
#define CHAOS_ENGINE_H

#include "core/chaos_types.h"
#include "core/chaos_error.h"
#include "core/chaos_time.h"
#include "core/chaos_log.h"
#include "platform/chaos_window.h"
#include "platform/chaos_input.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ce_engine_s;
typedef struct ce_engine_s ce_engine;

/**
 * @brief Callback invoked during engine initialization.
 */
typedef ce_bool (*ce_engine_init_fn)(ce_engine* engine, void* user_data);

/**
 * @brief Callback invoked during each fixed-timestep update.
 */
typedef void (*ce_engine_update_fn)(ce_engine* engine, void* user_data, ce_f32 dt);

/**
 * @brief Callback invoked to render a frame. Alpha is the interpolation factor in [0,1].
 */
typedef void (*ce_engine_render_fn)(ce_engine* engine, void* user_data, ce_f32 alpha);

/**
 * @brief Callback invoked before shutting down the engine.
 */
typedef void (*ce_engine_shutdown_fn)(ce_engine* engine, void* user_data);

/**
 * @brief Aggregated callbacks to run a game/application.
 */
typedef struct ce_engine_callbacks_s
{
    ce_engine_init_fn     on_init;
    ce_engine_update_fn   on_update;
    ce_engine_render_fn   on_render;
    ce_engine_shutdown_fn on_shutdown;
} ce_engine_callbacks;

/**
 * @brief Engine configuration structure.
 */
typedef struct ce_engine_config_s
{
    ce_window_config window;
    ce_u32           target_fps;
    ce_u32           max_updates_per_frame;
} ce_engine_config;

/**
 * @brief Runtime engine state.
 */
struct ce_engine_s
{
    ce_engine_config config;
    ce_engine_callbacks callbacks;
    ce_bool         running;
    ce_f32          fixed_dt;
    ce_f32          accumulator;
    ce_f32          elapsed_time;
    ce_u64          frame_index;
    ce_window       window;
    ce_input_state  input;
    void*           user_data;
};

/**
 * @brief Returns a default configuration with sensible values.
 */
ce_engine_config ce_engine_config_default(void);

/**
 * @brief Boots the engine and runs the main loop until exit.
 */
ce_result ce_engine_run(const ce_engine_config* config, const ce_engine_callbacks* callbacks, void* user_data);

/**
 * @brief Requests termination of the main loop at the next iteration.
 */
void ce_engine_request_exit(ce_engine* engine);

/**
 * @brief Accessor for the current input state.
 */
const ce_input_state* ce_engine_input(const ce_engine* engine);

/**
 * @brief Accessor for the engine window.
 */
ce_window* ce_engine_window(ce_engine* engine);

/**
 * @brief Returns the total elapsed time since the engine started.
 */
ce_f32 ce_engine_elapsed_time(const ce_engine* engine);

/**
 * @brief Returns the number of rendered frames.
 */
ce_u64 ce_engine_frame_index(const ce_engine* engine);

#ifdef __cplusplus
}
#endif

#endif /* CHAOS_ENGINE_H */
