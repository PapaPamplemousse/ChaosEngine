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
 * @file chaos_renderer_core.h
 * @brief Core renderer management: initialization, SDL2 + OpenGL context, and frame orchestration.
 *
 * This module owns the high-level lifecycle of the ChaosEngine renderer:
 * it initializes SDL2 and OpenGL, manages the main rendering loop, handles
 * window resizing, and provides the entry points for frame begin/end.
 *
 * It serves as the central coordination layer linking all rendering subsystems
 * (camera, meshes, shaders, textures, points, etc.).
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 */

#ifndef CHAOS_RENDERER_CORE_H
#define CHAOS_RENDERER_CORE_H

#define SDL_MAIN_HANDLED

/* ************* INCLUDE ************* */
#include "chaos_types.h"
#include "chaos_linalg.h"
#include "chaos_camera.h"
#include "chaos_shader.h" 

#include <SDL2/SDL.h>
#include <glad/gl.h>
/* ************* CONSTANTS ************* */

#define CHAOS_FEATURE_DEBUG_GRID   (1u << 0)
#define CHAOS_FEATURE_DEBUG_LINES  (1u << 1)

/* ************* ENUMERATIONS ************* */


typedef enum {
    CHAOS_BACKEND_OPENGL45 = 0
} chaos_backend_t;




/* ************* STRUCTURES ************* */

/**
 * @brief Renderer state (window, GL context, cached resources).
 */
typedef struct chaos_renderer_t {
    SDL_Window*     window;
    SDL_GLContext   gl_ctx;
    t_sint32        width, height;
    chaos_backend_t backend;
    t_uint32        features;

    /* OpenGL cached state/handles */
    t_uint32        _gl_default_fbo;
    t_uint32        _gl_prog_unlit_color;
    t_uint32        _gl_prog_unlit_tex;
    t_uint32        _gl_ubo_matrices;

    chaos_camera_t  camera;
    chaos_camera_control_t  camctl;
} chaos_renderer_t;

/* ************* UNIONS ************* */
/* ************* FUNCTIONS ************* */

/**
 * @brief Initialize the renderer and create an OpenGL 4.5 Core context.
 */
extern t_sint32 chaos_renderer_init(chaos_renderer_t* r,
                             const t_char* title,
                             t_sint32 width, t_sint32 height,
                             chaos_backend_t backend,
                             t_uint32 feature_flags);

/**
 * @brief Shutdown the renderer and destroy all internal GPU objects.
 */
extern void chaos_renderer_shutdown(chaos_renderer_t* r);

/**
 * @brief Handle window resize: update viewport and projection.
 */
extern void chaos_renderer_resize(chaos_renderer_t* r, t_sint32 width, t_sint32 height);

/**
 * @brief Begin a new frame (bind default FBO, clear color & depth).
 */
extern void chaos_begin_frame(chaos_renderer_t* r, t_float32 r_, t_float32 g_, t_float32 b_, t_float32 a_);

/**
 * @brief Complete the frame (swap buffers).
 */
extern void chaos_end_frame(chaos_renderer_t* r);

#endif /* CHAOS_RENDERER_CORE_H */