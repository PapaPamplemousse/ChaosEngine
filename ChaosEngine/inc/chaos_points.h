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
 * @file chaos_points.h
 * @brief Dynamic GPU point-stream API for particle rendering.
 *
 * Defines a flexible point buffer abstraction (`chaos_points_t`) designed
 * for streaming particle data each frame. Provides creation, destruction,
 * update, and draw routines that use GL_POINTS and custom shaders.
 *
 * Ideal for lightweight particle systems or debug point visualizations.
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 */

#ifndef CHAOS_POINTS_H
#define CHAOS_POINTS_H

 /* ************* INCLUDE ************* */
#include "chaos_types.h"
#include "chaos_renderer_core.h"

/* ************* CONSTANTS ************* */

/* ************* ENUMERATIONS ************* */


/* ************* STRUCTURES ************* */

/**
* @brief  forward declaration of chaos_renderer_t, to avoid chaos_renderer_core.h inclusion
*/
struct chaos_renderer_t; 
typedef struct chaos_renderer_t chaos_renderer_t;

/**
* @brief  forward declaration of chaos_shader_t, to avoid .h inclusion
*/
struct chaos_shader_t; 
typedef struct chaos_shader_t chaos_shader_t;

/**
 * @brief A dynamic stream of points (GL_POINTS) with the following layout:
 * - location 0: vec3 position
 * - location 1: float size      (world-space radius or logical size factor)
 * - location 2: float alpha     (0..1)
 *
 * Typical usage: create once with a capacity, stream per-frame data with
 * @ref chaos_points_update, then draw with @ref chaos_points_draw.
 */
typedef struct {
    t_uint32 _gl_vao;
    t_uint32 _gl_vbo;
    t_uint32 capacity; /* number of point vertices the buffer can hold */
} chaos_points_t;

/* ************* UNIONS ************* */


/* ************* FUNCTIONS ************* */
/**
 * @brief Create a dynamic point stream buffer with a fixed capacity.
 */
chaos_points_t chaos_points_create(t_uint32 capacity);

/**
 * @brief Destroy point stream GPU buffers.
 */
void chaos_points_destroy(chaos_points_t* ps);

/**
 * @brief Update the GPU buffer with @p count items from @p data.
 * @param stride_bytes Size of one item in bytes (e.g., sizeof(struct GPUParticle)).
 *
 * @details
 * Data must match the layout: vec3 pos, float size, float alpha. See @ref chaos_points_t.
 */
void chaos_points_update(const chaos_points_t* ps, const void* data, t_uint32 count, t_uint32 stride_bytes);

/**
 * @brief Draw the first @p count points with the given custom @p shader (GL_POINTS).
 * @note The shader is expected to use the UBO at binding 0 for view/proj.
 */
void chaos_points_draw(const chaos_points_t* ps, const chaos_shader_t* shader, t_uint32 count);

#endif /* CHAOS_POINTS_H */