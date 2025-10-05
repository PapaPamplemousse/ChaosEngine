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
 * @file chaosparticles.h
 * @brief Simple particle system definitions and simulation logic.
 *
 * This header defines the structures necessary for a particle system, including
 * the configurable behavior for a particle type (@ref particle_behavior_t) and 
 * the state of an individual particle instance (@ref particle_t).
 *
 * It provides inline functions for initialization and a basic simulation step,
 * incorporating gravity, air resistance, lifetime management, and simple ground
 * collision handling (bounce or destruction).
 * @note This module depends on the core physics functions (vectors, etc.) defined
 * in "physics.h" and "chaos_types.h".
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 * */
#ifndef CHAOS_PARTICLES_H
#define CHAOS_PARTICLES_H

#include "chaos_types.h"
#include "chaos_physics.h" 

/* ************* STRUCTURES ************* */

/**
 * @struct particle_behavior_t
 * @brief Describes the physical and lifetime behavior of a particle type.
 * * This structure allows for a generic, configurable particle system engine.
 */
typedef struct {
    vec_t gravity;         /**< @brief Gravity vector applied to the particle (e.g., world gravity). */
    t_float64 drag;         /**< @brief Air resistance coefficient (damping factor). */
    t_float64 lifetime;     /**< @brief Mean total lifespan of the particle (in seconds). */
    t_float64 mass;         /**< @brief Mass of the particle. */
    t_float64 restitution;  /**< @brief Coefficient of restitution (bounciness) for ground collisions. */
    t_bool destroy_on_ground;/**< @brief If TRUE, the particle is destroyed upon hitting the ground (Y=0); otherwise, it bounces. */
} particle_behavior_t;

/**
 * @struct particle_t
 * @brief Represents an independent particle instance.
 * * Stores the current state of a particle, including its motion, age, and 
 * a copy of its specific behavior rules.
 */
typedef struct {
    vec_t pos;           /**< @brief Current position vector. */
    vec_t vel;           /**< @brief Current velocity vector. */
    t_float64 age;       /**< @brief Time elapsed since the particle was initialized (in seconds). */
    t_float64 lifetime;  /**< @brief The total duration this specific particle will live. */
    t_float64 mass;      /**< @brief Particle mass (inherited from behavior, for generic physics). */
    t_bool active;       /**< @brief Status flag: TRUE if the particle is currently being simulated. */
    particle_behavior_t behavior; /**< @brief Physics and behavior parameters specific to this particle. */
} particle_t;


/* ************* INLINES ************* */

/**
 * @brief Initializes a generic particle instance.
 * * Sets the initial position, velocity, and copies the defined behavior
 * into the particle structure.
 * * @param p Pointer to the particle_t structure to initialize.
 * @param pos Initial position vector.
 * @param vel Initial velocity vector.
 * @param behavior Pointer to the particle_behavior_t defining its rules.
 */
static inline void particle_init(particle_t* p, vec_t pos, vec_t vel,const particle_behavior_t* behavior)
{
    p->pos = pos;
    p->vel = vel;
    p->age = 0.0;
    p->active = TRUE;
    p->behavior = *behavior;
    p->lifetime = behavior->lifetime;
    p->mass = behavior->mass;
}

/**
 * @brief Updates a particle using its defined physical behavior rules.
 * * Performs integration, checks lifetime expiration, and handles ground collision.
 * * @param p Pointer to the particle_t structure to update.
 * @param dt The time step (delta time) for the update (t_float64).
 */
static inline void particle_update(particle_t* p, t_float64 dt)
{
    if (!p->active) return;

    /* 1. Apply Gravity: v_new = v_old + (gravity * dt)
     Dimension is hardcoded to 3 for particle systems. */
    p->vel = vec_add(p->vel, vec_scale(p->behavior.gravity, dt, 3), 3);

    /* 2. Apply Air Resistance (simple multiplier): v_new = v_old * (1 - drag * dt) */
    p->vel = vec_scale(p->vel, 1.0 - p->behavior.drag * dt, 3);

   /* 3. Update Position: p_new = p_old + (v_new * dt) */
    p->pos = vec_add(p->pos, vec_scale(p->vel, dt, 3), 3);

    /* 4. Update Age and Check Lifetime */
    p->age += dt;
    if (p->age >= p->lifetime)
        p->active = FALSE; 

    /* 5. Ground Collision Check (Y=0 plane) */
    if (p->pos.y <= 0.0) {
        if (p->behavior.destroy_on_ground == TRUE) {
            p->active = FALSE;
        } else {
             /* Bounce */
            p->pos.y = 0.0;  /* Correct position to be exactly on the ground */ 
            p->vel.y *= -p->behavior.restitution;  /* Reverse and dampen vertical velocity */
        }
    }
}

#endif