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
 * @file chaos_shader.h
 * @brief GLSL shader program management and uniform utilities.
 *
 * Provides a thin abstraction over OpenGL shader programs, including creation,
 * destruction, binding, and uniform manipulation (mat4, vec4, float).
 *
 * Also defines the base `chaos_shader_t` type used throughout ChaosEngine.
 * Custom shaders are expected to use the standard UBO at binding 0:
 * @code
 * layout(std140, binding=0) uniform UBO { mat4 uView; mat4 uProj; };
 * @endcode
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 */

#ifndef CHAOS_SHADER_H
#define CHAOS_SHADER_H

 /* ************* INCLUDE ************* */
#include "chaos_types.h"
#include "chaos_renderer_core.h"

/* ************* CONSTANTS ************* */

/* ************* ENUMERATIONS ************* */


/* ************* STRUCTURES ************* */

/**
* @brief  forward declaration of chaos_renderer_t, to avoid haos_renderer_core.h inclusion
*/
struct chaos_renderer_t; 
typedef struct chaos_renderer_t chaos_renderer_t;

/**
 * @brief Compiled shader program wrapper.
 *
 * @details
 * Your custom shaders should include the following UBO to receive view/proj:
 * @code
 * layout(std140, binding=0) uniform UBO { mat4 uView; mat4 uProj; };
 * @endcode
 */
typedef struct chaos_shader_t{
    t_uint32 _gl_prog;
} chaos_shader_t;


/* ************* UNIONS ************* */


/* ************* GLOBAL ************* */

/* --- Built-in shaders (defined in chaos_shader.c) --- */
extern const char* VS_UNLIT_COLOR;
extern const char* FS_UNLIT_COLOR;
extern const char* VS_UNLIT_TEX;
extern const char* FS_UNLIT_TEX;



/* ************* FUNCTIONS ************* */

/* Internal helper for ChaosEngine shader creation */
t_uint32 gl_make_program(const char* vs_src, const char* fs_src);

/**
 * @brief Compile and link a shader program from GLSL source strings.
 */
chaos_shader_t chaos_shader_create(const char* vs_src, const char* fs_src);

/**
 * @brief Destroy a shader program.
 */
void chaos_shader_destroy(chaos_shader_t* s);

/* Uniform helpers (no hidden state) */
t_sint32 chaos_shader_uniform(const chaos_shader_t* s, const char* name);

/**
 * @brief Binds a ChaosEngine shader for subsequent rendering operations.
 *
 * Equivalent to glUseProgram(shader->_gl_prog), but keeps API consistent.
 *
 * @param shader Shader to bind.
 */
void chaos_shader_bind(const chaos_shader_t* shader);

void     chaos_shader_set_mat4(t_sint32 loc, const mat4* M);
void     chaos_shader_set_vec4(t_sint32 loc, const t_float32 rgba[4]);
void     chaos_shader_set_f32 (t_sint32 loc, t_float32 v);




#endif /* CHAOS_SHADER_H */