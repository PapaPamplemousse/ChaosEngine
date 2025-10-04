
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
 * @file chaosmath.h
 * @brief Fundamental mathematical structures and inline functions for ChaosEngine.
 *
 * This header defines core mathematical structures, notably the 'vec_t' union
 * for 2D/3D vectors, along with essential inline functions for vector creation
 * and basic operations (addition, scaling). It serves as the mathematical foundation
 * for the physics, graphics rendering, and other components of the engine.
 * The use of array operations (data[i]) allows vector handling in both 2D and 3D
 * dimensions, controlled by the 'dim' parameter in the functions.
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 * */
#ifndef CHAOS_MATH_H
#define CHAOS_MATH_H

#include "chaostypes.h"


/* ************* UNIONS ************* */
/**
 * @union vec_t
 * @brief Represents a vector in 2D or 3D space.
 * 
 * A union that allows accessing vector components either as named fields x, y, z
 * or as an array data[3]. The dimension in use is determined by the 'dim' field
 * in the body_t structure.
 */
typedef union {
    struct { t_float64 x, y, z; };
    t_float64 data[3];
} vec_t;


/* ************* INLINES ************* */

/**
 * @brief Creates a 2D vector with z=0.
 *
 * @param x X component
 * @param y Y component
 * @return vec_t Vector with x, y and z=0
 */
static inline vec_t vec2(t_float64 x, t_float64 y) {
    return (vec_t){ .x = x, .y = y, .z = 0.0 };
}

/**
 * @brief Creates a 3D vector.
 *
 * @param x X component
 * @param y Y component
 * @param z Z component
 * @return vec_t Vector with x, y, z
 */
static inline vec_t vec3(t_float64 x, t_float64 y, t_float64 z) {
    return (vec_t){ .x = x, .y = y, .z = z };
}

/**
 * @brief Adds two vectors together component-wise.
 * 
 * @param a First vector.
 * @param b Second vector.
 * @param dim Dimension to use (2 or 3).
 * @return vec_t Resulting vector (a + b).
 */
static inline vec_t vec_add(vec_t a, vec_t b, t_uint8 dim) {
    vec_t r;
    for (t_uint8 i = 0; i < dim; ++i) r.data[i] = a.data[i] + b.data[i];
    return r;
}

/**
 * @brief Scales a vector by a scalar.
 * 
 * @param a Vector to scale.
 * @param s Scalar multiplier.
 * @param dim Dimension to use (2 or 3).
 * @return vec_t Resulting scaled vector.
 */
static inline vec_t vec_scale(vec_t a, t_float64 s, t_uint8 dim) {
    vec_t r;
    for (t_uint8 i = 0; i < dim; ++i) r.data[i] = a.data[i]*s;
    return r;
}

#endif 
