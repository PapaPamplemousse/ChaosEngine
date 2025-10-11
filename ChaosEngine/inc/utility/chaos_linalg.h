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
 * @file chaos_linalg.h
 * @brief Structures and operations for **Linear Algebra** (Vectors and Matrices) in ChaosEngine.
 *
 * This header defines the core structures like **ce_vec3_t** (3D Vector) and **ce_mat4_t** (4x4 Matrix)
 * and provides inline functions for fundamental **vector operations** (addition, scaling, dot, cross,
 * normalization) and **matrix operations** (multiplication, identity, perspective, look-at,
 * point transformation). It is the foundation for 3D graphics and physics transformations.
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 * */
#ifndef CHAOS_LINALG_H
#define CHAOS_LINALG_H

#include "core/chaos_types.h"

/* ************* STRUCTURES ************* */

/**
 * @brief 2D vector structure.
 */
typedef struct ce_vec2_s
{
    ce_f32 x; /*< X component */
    ce_f32 y; /*< Y component */
} ce_vec2_t;

/**
 * @brief 3D vector structure.
 */
typedef struct ce_vec3_s
{
    ce_f32 x; /*< X component */
    ce_f32 y; /*< Y component */
    ce_f32 z; /*< Z component */
} ce_vec3_t;

/**
 * @brief 4D vector structure.
 */
typedef struct ce_vec4_s
{
    ce_f32 x; /*< X component */
    ce_f32 y; /*< Y component */
    ce_f32 z; /*< Z component */
    ce_f32 w; /*< W component (homogeneous coordinate) */
} ce_vec4_t;

/**
 * @brief 4x4 matrix structure in row-major order.
 */
typedef struct ce_mat4_s
{
    ce_f32 m[4][4]; /*< 4x4 matrix elements */
} ce_mat4_t;

/* ************* VECTOR OPERATIONS ************* */

/**
 * @brief Adds two 3D vectors component-wise.
 * @param a The first vector (ce_vec3_t).
 * @param b The second vector (ce_vec3_t).
 * @return The resulting vector (ce_vec3_t).
 */
ce_vec3_t ce_vec3_add(const ce_vec3_t a, const ce_vec3_t b);

/**
 * @brief Subtracts vector b from vector a component-wise.
 * @param a The first vector (ce_vec3_t).
 * @param b The second vector to subtract (ce_vec3_t).
 * @return The resulting vector (ce_vec3_t).
 */
ce_vec3_t ce_vec3_sub(const ce_vec3_t a, const ce_vec3_t b);

/**
 * @brief Scales a 3D vector by a scalar.
 * @param a The vector to scale (ce_vec3_t).
 * @param s The scalar value (ce_f32).
 * @return The scaled vector (ce_vec3_t).
 */
ce_vec3_t ce_vec3_scale(const ce_vec3_t a, ce_f32 s);

/**
 * @brief Computes the dot product of two 3D vectors.
 * @param a The first vector (ce_vec3_t).
 * @param b The second vector (ce_vec3_t).
 * @return The dot product (ce_f32).
 */
ce_f32 ce_vec3_dot(const ce_vec3_t a, const ce_vec3_t b);

/**
 * @brief Computes the cross product of two 3D vectors.
 * @param a The first vector (ce_vec3_t).
 * @param b The second vector (ce_vec3_t).
 * @return The resulting vector (ce_vec3_t).
 */
ce_vec3_t ce_vec3_cross(const ce_vec3_t a, const ce_vec3_t b);

/**
 * @brief Normalizes a 3D vector to unit length.
 * @param a The vector to normalize (ce_vec3_t).
 * @return The normalized vector (ce_vec3_t). If the input vector has zero length, it is returned unchanged.
 */
ce_vec3_t ce_vec3_normalize(const ce_vec3_t a);

/* ************* MATRIX OPERATIONS ************* */

/**
 * @brief Creates a 4x4 identity matrix.
 * @return The identity matrix (ce_mat4_t).
 */
ce_mat4_t ce_mat4_identity(void);

/**
 * @brief Multiplies two 4x4 matrices.
 * @param a The first matrix (ce_mat4_t).
 * @param b The second matrix (ce_mat4_t).
 * @return The resulting matrix (ce_mat4_t).
 */
ce_mat4_t ce_mat4_mul(const ce_mat4_t a, const ce_mat4_t b);

/**
 * @brief Multiplies a 4x4 matrix by a 4D vector.
 * @param m The matrix (ce_mat4_t).
 * @param v The vector (ce_vec4_t).
 * @return The resulting vector (ce_vec4_t).
 */
ce_vec4_t ce_mat4_mul_vec4(const ce_mat4_t m, const ce_vec4_t v);

/**
 * @brief Creates a perspective projection matrix.
 * @param fov_rad The field of view in radians (ce_f32).
 * @param aspect The aspect ratio (width/height) (ce_f32).
 * @param znear The near clipping plane distance (ce_f32).
 * @param zfar The far clipping plane distance (ce_f32).
 * @return The perspective projection matrix (ce_mat4_t).
 */
ce_mat4_t ce_mat4_perspective(ce_f32 fov_rad, ce_f32 aspect, ce_f32 znear, ce_f32 zfar);

/**
 * @brief Creates a look-at view matrix.
 * @param eye The position of the camera (ce_vec3_t).
 * @param center The point the camera is looking at (ce_vec3_t).
 * @param up The up direction vector (ce_vec3_t).
 * @return The look-at view matrix (ce_mat4_t).
 */
ce_mat4_t ce_mat4_look_at(const ce_vec3_t eye, const ce_vec3_t center, const ce_vec3_t up);

#endif /* CHAOS_LINALG_H */
