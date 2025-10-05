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
 * @file chaos_camera.h
 * @brief Camera setup, control, and view/projection UBO management.
 *
 * Provides camera manipulation functions (orbit, zoom, reset) and manages
 * the synchronization of view/projection matrices with the GPU via a shared
 * uniform buffer object (binding = 0).
 *
 * Used by both 3D and 2D rendering systems to control perspective or orthographic
 * projection behavior.
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 */

#ifndef CHAOS_CAMERA_H
#define CHAOS_CAMERA_H

 /* ************* INCLUDE ************* */
#include "chaos_types.h"

/* ************* CONSTANTS ************* */

#define CHAOS_FEATURE_DEBUG_GRID   (1u << 0)
#define CHAOS_FEATURE_DEBUG_LINES  (1u << 1)

/* ************* ENUMERATIONS ************* */


/* ************* STRUCTURES ************* */

/**
* @brief  forward declaration of chaos_renderer_t, to avoid haos_renderer_core.h inclusion
*/
struct chaos_renderer_t; 
typedef struct chaos_renderer_t chaos_renderer_t;

/**
 * @brief Perspective camera with cached view/projection matrices.
 */
typedef struct {
    vec_t position;
    vec_t target;
    vec_t up;
    t_float64 fov_deg, nearp, farp;
    mat4 view, proj;
    t_uint8 dim; /* 3 */
} chaos_camera_t;

typedef struct {
    t_float32 yaw;
    t_float32 pitch;
    t_float32 distance;
    t_bool    orbit_mode;
} chaos_camera_control_t;

/* ************* UNIONS ************* */


/* ************* FUNCTIONS ************* */

/**
 * @brief Configure the camera and upload view/projection to the UBO.
 */
extern void chaos_camera_setup(chaos_renderer_t* r,
                        vec_t eye, vec_t target, vec_t up,
                        t_float64 fov_deg, t_float64 nearp, t_float64 farp);

/**
 * @brief Orbit the camera horizontally and vertically around its target.
 *
 * @param r Renderer containing the active camera.
 * @param dYaw Degrees to rotate horizontally (left/right).
 * @param dPitch Degrees to rotate vertically (up/down).
 */
extern void chaos_camera_orbit(chaos_renderer_t* r, t_float32 dYaw, t_float32 dPitch);

/**
 * @brief Zoom the camera in or out (moves closer or farther from target).
 *
 * @param r Renderer containing the active camera.
 * @param delta Positive to zoom out, negative to zoom in.
 */
extern void chaos_camera_zoom(chaos_renderer_t* r, t_float32 delta);

/**
 * @brief Reset the camera to a default position and orientation.
 *
 * Default eye = (6, 6, 10), target = (0, 0, 0), up = (0, 1, 0).
 */
extern void chaos_camera_reset(chaos_renderer_t* r);

/**
 * @brief Compute the recommended GL point-size scale factor for point sprites.
 *
 * @details
 * Final on-screen point size (in pixels) should follow:
 * @code
 * pointSizePixels = (pointScale * worldSize) / viewSpaceDistance;
 * @endcode
 */
extern t_float32 chaos_compute_point_scale(const chaos_renderer_t* r);

/**
 * @brief Uploads the View and Projection matrices to the GPU's Uniform Buffer Object (UBO).
 *
 * This function fetches the current view and projection matrices from the renderer's
 * camera, converts them to the necessary format (typically float array), and 
 * updates the designated Uniform Buffer Object (UBO) on the graphics card.
 *
 * @param r Pointer to the chaos_renderer_t context, which holds the camera matrices 
 * and the GPU UBO handle.
 */
extern void chaos_upload_vp_ubo(chaos_renderer_t* r);

#endif /* CHAOS_CAMERA_H */