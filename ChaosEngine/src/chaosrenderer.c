#include "chaosrenderer.h"
#include <math.h>

/* ====== Initialisation / Destruction ====== */

/**
 * @brief Initializes the rendering system, creating the window and renderer.
 * * @param r Pointer to the renderer_t structure to initialize.
 * @param title The title of the window.
 * @param width The desired width of the window.
 * @param height The desired height of the window.
 * @return t_sint32 Returns 0 on success, or a non-zero error code on failure.
 */
t_sint32 renderer_init(renderer_t* r, const t_char* title, t_sint32 width, t_sint32 height) 
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

    r->window = SDL_CreateWindow(title,
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height, SDL_WINDOW_SHOWN);
    if (!r->window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        return -1;
    }

    r->renderer = SDL_CreateRenderer(r->window, -1,
                                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!r->renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        // Clean up the window if renderer creation fails
        SDL_DestroyWindow(r->window);
        SDL_Quit();
        return -1;
    }

    // Assign dimensions using normalized types
    r->width = width;
    r->height = height;
    
    // Initialize default camera position
    // Note: The vec_t union initialization { {x, y, z} } is C99 syntax, but 
    // it's safer to use the .x, .y, .z syntax for full portability with vec_t.
    // Assuming vec_t is defined to support either array or struct access.
    // The original C99 union initialization style is preserved here.
    r->camera.position = (vec_t){.x = 0.0, .y = 5.0, .z = 10.0};
    r->camera.target   = (vec_t){.x = 0.0, .y = 0.0, .z = 0.0};
    r->camera.fov = 60.0; // t_float64 used implicitly

    return 0;
}

/**
 * @brief Cleans up and destroys the rendering context (window and renderer).
 *
 * @param r Pointer to the renderer_t structure to destroy.
 */
void renderer_destroy(renderer_t* r) {
    if (r->renderer)
        SDL_DestroyRenderer(r->renderer);
    if (r->window)
        SDL_DestroyWindow(r->window);
    SDL_Quit();
}

/* ====== Caméra ====== */

/**
 * @brief Sets the position of the rendering camera.
 *
 * @param r Pointer to the renderer context.
 * @param pos The new world position vector for the camera.
 */
void camera_set_position(renderer_t* r, vec_t pos) { r->camera.position = pos; }

/**
 * @brief Makes the camera point towards a specific target in the world.
 *
 * @param r Pointer to the renderer context.
 * @param target The world position vector the camera should look at.
 */
void camera_look_at(renderer_t* r, vec_t target) { r->camera.target = target; }

/* ====== Rendu ====== */

/**
 * @brief Clears the current rendering target (the screen/window).
 *
 * Typically fills the target with the background color (usually black).
 * @param r Pointer to the renderer context.
 */
void renderer_clear(renderer_t* r) {
    // Colors are passed as t_uint8, but SDL expects int for its components
    // (though in the range 0-255). 0, 0, 0, 255 is safe.
    SDL_SetRenderDrawColor(r->renderer, 0, 0, 0, 255);
    SDL_RenderClear(r->renderer);
}

/**
 * @brief Presents the rendered content to the screen.
 *
 * Swaps the back buffer to the front buffer, making the drawing visible.
 * @param r Pointer to the renderer context.
 */
void renderer_present(renderer_t* r) { SDL_RenderPresent(r->renderer); }

/* ====== Projection simple 3D -> 2D ====== */

/**
 * @brief Projects a 3D world coordinate onto the 2D screen coordinate space.
 * * Uses a simplified perspective projection model.
 *
 * @param pos The 3D world position to project.
 * @param r Pointer to the renderer context (contains camera and screen info).
 * @param sx Output pointer for the screen X coordinate (t_sint32).
 * @param sy Output pointer for the screen Y coordinate (t_sint32).
 */
static void project(vec_t pos, renderer_t* r, t_sint32* sx, t_sint32* sy) {
    // Calculate the perspective scale factor. 
    // Note: This simple method doesn't account for camera rotation (only position).
    t_float64 scale = r->camera.fov / (pos.z - r->camera.position.z + r->camera.fov);
    
    // Calculate screen x coordinate
    *sx = (t_sint32)(pos.x * scale * r->width / 2 + r->width / 2);
    
    // Calculate screen y coordinate (y axis is typically inverted on screen: -pos.y)
    *sy = (t_sint32)(-pos.y * scale * r->height / 2 + r->height / 2);
}

/* ====== Dessin des primitives ====== */

/**
 * @brief Draws a single primitive object to the current rendering target.
 *
 * The actual rendering logic depends on the primitive type (sphere, cube, etc.).
 * @param r Pointer to the renderer context.
 * @param p Pointer to the primitive_t object to draw.
 */
void draw_primitive(renderer_t* r, primitive_t* p) {
    t_sint32 sx, sy;
    project(p->position, r, &sx, &sy);

    // Set draw color using t_uint8 color components
    SDL_SetRenderDrawColor(r->renderer, 
                           p->color[0], // R (t_uint8)
                           p->color[1], // G (t_uint8)
                           p->color[2], // B (t_uint8)
                           255);        // A (t_uint8)

    // Calculate size based on scale.x (t_float64)
    t_sint32 size = (t_sint32)(p->scale.x * 10.0);
    
    // SDL_Rect uses int (t_sint32 in our scheme)
    SDL_Rect rect = { 
        sx - size / 2, 
        sy - size / 2, 
        size, 
        size 
    };

    SDL_RenderFillRect(r->renderer, &rect);
}

/**
 * @brief Renders an entire collection of primitives (the scene).
 *
 * @param r Pointer to the renderer context.
 * @param objects A pointer to an array of pointers to primitive_t objects.
 * @param count The number of primitives in the array.
 */
void draw_scene(renderer_t* r, primitive_t** objects, t_sint32 count) {
    for (t_sint32 i = 0; i < count; i++)
        draw_primitive(r, objects[i]);
}