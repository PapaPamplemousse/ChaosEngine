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
 * @file chaosphysics
 * @brief Simple physics simulation structures and functions supporting 2D and 3D.
 *
 * This header defines basic types and inline functions to represent vectors, physical bodies,
 * and a simulation world with gravity. It includes vector arithmetic operations, body initialization,
 * force application, and a simple Euler integration step for advancing the simulation.
 * The dimension (2D or 3D) is determined by the 'dim' field in body_t.
 * 
 * This file is suitable for lightweight physics simulations in games or educational projects.
 * 
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 * 
 */
#ifndef CHAOS_PHYSICS_H
#define CHAOS_PHYSICS_H

#include "chaostypes.h"
#include "chaosmath.h"

/* ************* STRUCTURES ************* */

/**
 * @struct body_t
 * @brief Represents a physical body in 2D or 3D space.
 * 
 * A body has a position, velocity, accumulated force, mass, and a dimension flag.
 * The dimension flag 'dim' determines whether the body is treated as 2D or 3D.
 */
typedef struct {
    t_uint8 dim;      
    vec_t pos;
    vec_t vel;
    vec_t force;
    t_float64 mass;
} body_t;


/**
 * @struct world_t
 * @brief Represents the simulation world containing environmental parameters.
 * 
 * Currently only supports gravity, which is applied to bodies each simulation step.
 */
typedef struct {
    vec_t gravity;
} world_t;


/* ************* INLINES ************* */

/**
 * @brief Initializes a physical body.
 * 
 * Sets mass, dimension, initial position, and zeroes velocity and force vectors.
 * 
 * @param b Pointer to the body to initialize.
 * @param mass Mass of the body.
 * @param pos Initial position vector.
 * @param dim Dimension (2 for 2D, 3 for 3D).
 */
static inline void body_init(body_t *b, t_float64 mass, vec_t pos, t_uint8 dim) {
    b->mass = mass;
    b->dim = dim;
    b->pos = pos;
    for (t_uint8 i = 0; i < dim; ++i) { b->vel.data[i] = 0; b->force.data[i] = 0; }
}

/**
 * @brief Applies a force to a body.
 * 
 * Adds the given force vector to the body's accumulated force.
 * 
 * @param b Pointer to the body.
 * @param f Force vector to apply.
 */
static inline void body_apply_force(body_t *b, vec_t f) {
    for (t_uint8 i = 0; i < b->dim; ++i) b->force.data[i] += f.data[i];
}

/**
 * @brief Advances the body's state using simple Euler integration.
 * 
 * Updates velocity and position based on accumulated forces and clears the force vector.
 * 
 * @param b Pointer to the body.
 * @param dt Time step in seconds.
 */
static inline void integrate_euler(body_t *b, t_float64 dt) {
    if (b->mass <= 0) return;
    vec_t acc = vec_scale(b->force, 1.0 / b->mass, b->dim);
    b->vel = vec_add(b->vel, vec_scale(acc, dt, b->dim), b->dim);
    b->pos = vec_add(b->pos, vec_scale(b->vel, dt, b->dim), b->dim);
    for (t_uint8 i = 0; i < b->dim; ++i) b->force.data[i] = 0;
}

/**
 * @brief Initializes the simulation world.
 * 
 * @param w Pointer to the world structure.
 * @param gravity Gravity vector to apply to bodies.
 */
static inline void world_init(world_t *w, vec_t gravity) { w->gravity = gravity; }

/**
 * @brief Performs a simulation step for a single body.
 * 
 * Applies gravity to the body and integrates its motion.
 * 
 * @param w Pointer to the world.
 * @param b Pointer to the body to update.
 * @param dt Time step in seconds.
 */
static inline void world_step(world_t *w, body_t *b, t_float64 dt) {
    vec_t g = vec_scale(w->gravity, b->mass, b->dim);
    body_apply_force(b, g);
    integrate_euler(b, dt);
}

#endif
