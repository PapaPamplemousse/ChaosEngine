
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
 * @file chaosrenderer.h
 * @brief Graphics rendering module for the Chaos Engine, primarily using SDL2.
 *
 * This header defines the structures and interface for managing the graphical
 * output of the engine. It includes structures for the rendering context (@ref renderer_t),
 * the virtual camera (@ref camera_t), and the primitives that can be drawn
 * (@ref primitive_t).
 *
 * It provides functions for initialization, cleanup, camera manipulation, and drawing
 * both individual primitives and entire scenes. It utilizes SDL2 for window and
 * hardware rendering context management.
 * @note This file depends on the 'chaosmath.h' for vector operations and 'chaostypes.h'
 * for basic type definitions.
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 * */

#ifndef CHAOS_RENDERER_H
#define CHAOS_RENDERER_H

#include <SDL2/SDL.h>
#include "chaosmath.h"
#include "chaostypes.h"


/* ************* ENUMERATIONS ************* */

/**
 * @enum primitive_type_t
 * @brief Enumerates the supported types of graphical primitives.
 */
typedef enum {
    PRIMITIVE_SPHERE, /**< A basic sphere shape. */
    PRIMITIVE_PLANE,  /**< An infinite or finite plane. */
    PRIMITIVE_CUBE,   /**< A 3D cube or box shape. */
    PRIMITIVE_LINE    /**< A line segment. */
} primitive_type_t;

/* ************* STRUCTURES ************* */

/**
 * @struct camera_t
 * @brief Defines a virtual camera for the renderer.
 *
 * It stores the camera's world position, the point it is looking at, and
 * its field of view (FOV).
 */
typedef struct {
    vec_t position; /**< World coordinates of the camera. */
    vec_t target;   /**< World coordinates the camera is focused on. */
    t_float64 fov;  /**< Field of view angle (e.g., in degrees). */
} camera_t;



/**
 * @struct primitive_t
 * @brief Defines a drawable geometric object in the scene.
 *
 * Contains the type, spatial properties (position, scale), and color of the object.
 */
typedef struct {
    primitive_type_t type; /**< The geometric shape type. */
    vec_t position;        /**< World coordinates of the primitive's center. */
    vec_t scale;           /**< Scaling factors (size) along X, Y, Z. */
    t_uint8 color[3];      /**< RGB color values (0-255). */
} primitive_t;

/**
 * @struct renderer_t
 * @brief The main rendering context structure.
 *
 * Holds the SDL window and renderer handles, the screen dimensions, and the
 * current camera settings.
 */
typedef struct {
    SDL_Window* window;      /**< Pointer to the SDL window. */
    SDL_Renderer* renderer;  /**< Pointer to the SDL hardware renderer context. */
    t_sint32 width, height;  /**< Dimensions (in pixels) of the rendering surface. */
    camera_t camera;         /**< Current camera state. */
} renderer_t;
/* ====== Interface ====== */

/**
 * @brief Initializes the rendering system, creating the window and renderer.
 *
 * @param r Pointer to the renderer_t structure to initialize.
 * @param title The title of the window.
 * @param width The desired width of the window.
 * @param height The desired height of the window.
 * @return t_sint32 Returns 0 on success, or a non-zero error code on failure.
 */
t_sint32 renderer_init(renderer_t* r, const t_char* title, t_sint32 width, t_sint32 height);

/**
 * @brief Cleans up and destroys the rendering context (window and renderer).
 *
 * @param r Pointer to the renderer_t structure to destroy.
 */
void renderer_destroy(renderer_t* r);

/**
 * @brief Sets the position of the rendering camera.
 *
 * @param r Pointer to the renderer context.
 * @param pos The new world position vector for the camera.
 */
void camera_set_position(renderer_t* r, vec_t pos);

/**
 * @brief Makes the camera point towards a specific target in the world.
 *
 * @param r Pointer to the renderer context.
 * @param target The world position vector the camera should look at.
 */
void camera_look_at(renderer_t* r, vec_t target);

/**
 * @brief Clears the current rendering target (the screen/window).
 *
 * Typically fills the target with the background color (usually black).
 * @param r Pointer to the renderer context.
 */
void renderer_clear(renderer_t* r);


/**
 * @brief Presents the rendered content to the screen.
 *
 * Swaps the back buffer to the front buffer, making the drawing visible.
 * @param r Pointer to the renderer context.
 */
void renderer_present(renderer_t* r);

/**
 * @brief Draws a single primitive object to the current rendering target.
 *
 * The actual rendering logic depends on the primitive type (sphere, cube, etc.).
 * @param r Pointer to the renderer context.
 * @param p Pointer to the primitive_t object to draw.
 */
void draw_primitive(renderer_t* r, primitive_t* p);


/**
 * @brief Renders an entire collection of primitives (the scene).
 *
 * @param r Pointer to the renderer context.
 * @param objects A pointer to an array of pointers to primitive_t objects.
 * @param count The number of primitives in the array.
 */
void draw_scene(renderer_t* r, primitive_t** objects, t_sint32 count);


#endif
