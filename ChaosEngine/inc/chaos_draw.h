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
 * @file chaos_draw.h
 * @brief Generic draw utilities and debug visualization helpers.
 *
 * Contains reusable draw routines for standard meshes, sprites, and
 * debug primitives such as lines and grids. Handles built-in unlit
 * materials (color or textured) and integrates cleanly with the
 * camera subsystem.
 *
 * These functions never modify engine state beyond OpenGL draw calls.
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 */

#ifndef CHAOS_DRAW_H
#define CHAOS_DRAW_H

 /* ************* INCLUDE ************* */
#include "chaos_types.h"

/* ************* CONSTANTS ************* */

/* ************* ENUMERATIONS ************* */

/**
 * @brief Material type: unlit solid color or unlit texture.
 */
typedef enum {
    CHAOS_MAT_UNLIT_COLOR = 0,
    CHAOS_MAT_UNLIT_TEXTURE = 1
} chaos_material_type_t;

/* ************* STRUCTURES ************* */

/**
* @brief  forward declaration of chaos_renderer_t, to avoid chaos_renderer_core.h inclusion
*/
struct chaos_renderer_t;
typedef struct chaos_renderer_t chaos_renderer_t;

/**
* @brief  forward declaration of chaos_texture_t, to avoid .h inclusion
*/
struct chaos_texture_t;
typedef struct chaos_texture_t chaos_texture_t;

/**
* @brief  forward declaration of chaos_mesh_t, to avoid .h inclusion
*/
struct chaos_mesh_t;
typedef struct chaos_mesh_t chaos_mesh_t;



/**
 * @brief Draw material with an optional texture and RGBA color multiplier.
 */
typedef struct {
    chaos_material_type_t type;
    t_float32 color[4];
    const chaos_texture_t* tex; /* optional */
} chaos_material_t;

/* ************* UNIONS ************* */


/* ************* FUNCTIONS ************* */
/**
 * @brief Draw a mesh with a simple unlit material and model matrix.
 */
void chaos_draw_mesh(chaos_renderer_t* r,
                     const chaos_mesh_t* mesh,
                     const chaos_material_t* mat,
                     const mat4* model);

/* Debug helpers (transient, for gizmos) */
void chaos_debug_draw_grid(chaos_renderer_t* r, t_float32 half_x, t_float32 half_z, t_float32 step, t_float32 y);
void chaos_debug_draw_line3(chaos_renderer_t* r, vec_t a, vec_t b, t_float32 rgba[4]);

/* 2D sprite helper (orthographic auto-proj) */
void chaos_draw_sprite2D(chaos_renderer_t* r, const chaos_texture_t* tex, t_float32 x, t_float32 y, t_float32 w, t_float32 h, t_float32 rgba[4]);
#endif /* CHAOS_DRAW_H */