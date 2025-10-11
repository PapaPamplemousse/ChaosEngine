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
 * @file chaos_scene.h
 * @author PapaPamplemousse
 * @brief Simple 3D scene representation with camera and meshes.
 * @version 1.0
 * @date October 2025
 *
 */
#ifndef CHAOS_SCENE_H
#define CHAOS_SCENE_H

#include "core/chaos_types.h"
#include "utility/chaos_math.h"
#include "utility/chaos_linalg.h"
#include "gfx/chaos_gfx_types.h"

/* ************* CONSTANTS ************* */
#define CHAOS_SCENE_MAX_MESHES (16U)

/* Forward declaration to avoid including SDL headers here */
struct SDL_Renderer;

/* ************* STRUCTURES ************* */

/**
 * @brief Camera structure for orbiting around a target point.
 */
typedef struct ce_camera_s
{
    ce_f32    yaw;      /*< Yaw angle in radians */
    ce_f32    pitch;    /*< Pitch angle in radians */
    ce_f32    distance; /*< Distance from the target point */
    ce_vec3_t target;   /*< Target point the camera orbits around */
    ce_f32    fov_rad;  /*< Field of view in radians */
    ce_f32    znear;    /*< Near clipping plane */
    ce_f32    zfar;     /*< Far clipping plane */
} ce_camera_t;

/**
 * @brief Simple 3D scene with a camera and multiple meshes.
 */
typedef struct ce_scene_s
{
    ce_camera_t cam;                            /*< Scene camera */
    ce_mesh_t*  meshes[CHAOS_SCENE_MAX_MESHES]; /*< Array of pointers to meshes */
    ce_u32      mesh_count;                     /*< Current number of meshes in the scene */
} ce_scene_t;

/* ************* FUNCTION DECLARATIONS ************* */

/**
 * @brief Initializes a scene with default camera parameters and no meshes.
 * @param s Pointer to the ce_scene_t structure to initialize.
 * @return None.
 */
void ce_scene_init(ce_scene_t* s);

/**
 * @brief Adds a mesh to the scene if there is capacity.
 * @param s Pointer to the ce_scene_t structure.
 * @param m Pointer to the ce_mesh_t structure to add.
 * @return None.
 */
void ce_scene_add_mesh(ce_scene_t* s, ce_mesh_t* m);

/**
 * @brief Updates the camera's position based on yaw, pitch, and distance.
 * @param cam Pointer to the ce_camera_t structure to update.
 * @param dyaw Change in yaw angle in radians.
 * @param dpitch Change in pitch angle in radians.
 * @param ddist Change in distance from the target point.
 * @return None.
 */
void ce_camera_orbit(ce_camera_t* cam, ce_f32 dyaw, ce_f32 dpitch, ce_f32 ddist);

/**
 * @brief Renders the scene using a simple software rasterizer (SDL2).
 * @param s Pointer to the ce_scene_t structure to render.
 * @param rnd Pointer to the SDL_Renderer to draw into.
 * @param w Width of the rendering area in pixels.
 * @param h Height of the rendering area in pixels.
 * @return None.
 */
void ce_scene_render_software(ce_scene_t* s, struct SDL_Renderer* rnd, ce_u32 w, ce_u32 h);

/**
 * @brief Fits the camera to see the given bounds (centered & fully visible).
 * @param cam Camera to modify.
 * @param b   Bounds to frame.
 */
void ce_camera_fit_bounds(ce_camera_t* cam, const ce_bounds3_t* b);

/**
 * @brief Translate (pan) the camera target in view plane.
 * @param cam Camera pointer.
 * @param delta_x Horizontal drag (pixels or arbitrary units).
 * @param delta_y Vertical drag.
 * @param speed Pan sensitivity factor (world units per screen unit).
 */
void ce_camera_pan(ce_camera_t* cam, ce_f32 delta_x, ce_f32 delta_y, ce_f32 speed);

#endif
