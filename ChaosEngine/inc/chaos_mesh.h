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
 * @file chaos_mesh.h
 * @brief GPU mesh abstraction (VAO/VBO/IBO) and helper generators.
 *
 * Defines the mesh data structure and provides creation/destruction helpers
 * for interleaved vertex buffers and optional indices. Includes utility functions
 * to procedurally generate simple meshes such as UV spheres.
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 */

#ifndef CHAOS_MESH_H
#define CHAOS_MESH_H

 /* ************* INCLUDE ************* */
#include "chaos_types.h"

/* ************* CONSTANTS ************* */

/* ************* ENUMERATIONS ************* */


/* ************* STRUCTURES ************* */
/**
 * @brief Interleaved mesh: position[3], optional normal[3], optional uv[2].
 */
typedef struct chaos_mesh_t{
    t_uint32 vertex_count;
    t_uint32 index_count; /* 0 if non-indexed */
    t_uint32 _priv_gl_vao, _priv_gl_vbo, _priv_gl_ibo;
} chaos_mesh_t;

/* ************* UNIONS ************* */


/* ************* FUNCTIONS ************* */
/**
 * @brief Create a static mesh. Any of normals/uvs/indices may be NULL.
 */
extern chaos_mesh_t chaos_mesh_create(const t_float32* positions,
                               const t_float32* normals,
                               const t_float32* uvs,
                               t_uint32 vertex_count,
                               const t_uint32* indices,
                               t_uint32 index_count);
                               
/**
* @brief Destroy a static mesh
 */
extern void chaos_mesh_destroy(chaos_mesh_t* m);

/**
 * @brief Creates a simple UV sphere mesh (positions + indices only).
 *
 * @param radius Sphere radius
 * @param slices Number of longitudinal subdivisions (around Y)
 * @param stacks Number of latitudinal subdivisions (top to bottom)
 * @return chaos_mesh_t The generated sphere mesh
 */
extern chaos_mesh_t chaos_mesh_create_sphere(t_float32 radius, t_uint32 slices, t_uint32 stacks);


chaos_mesh_t chaos_mesh_create_cube(t_float32 size);

#endif /* CHAOS_MESH_H */