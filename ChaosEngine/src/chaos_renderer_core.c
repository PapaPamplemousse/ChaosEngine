#include "chaos_renderer_core.h"
#include "chaos_camera.h"
#include "chaos_shader.h"
#include "chaos_mesh.h"
#include "chaos_texture.h"
#include "chaos_points.h"
#include "chaos_draw.h"

#include <SDL2/SDL.h>
#include <glad/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* -------------------------------------------------------------------------- */
/* View/Projection UBO                                                        */
/* -------------------------------------------------------------------------- */

struct UBO_Matrices { t_float32 view[16]; t_float32 proj[16]; };

void chaos_upload_vp_ubo(chaos_renderer_t* r) {
    struct UBO_Matrices data;
    m4_to_f32(&r->camera.view, data.view);
    m4_to_f32(&r->camera.proj, data.proj);
    glBindBuffer(GL_UNIFORM_BUFFER, r->_gl_ubo_matrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof data, &data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

/* -------------------------------------------------------------------------- */
/* Public API                                                                 */
/* -------------------------------------------------------------------------- */

t_sint32 chaos_renderer_init(chaos_renderer_t* r,
                             const t_char* title,
                             t_sint32 width, t_sint32 height,
                             chaos_backend_t backend,
                             t_uint32 feature_flags)
{
    memset(r, 0, sizeof *r);
    r->backend  = backend;
    r->features = feature_flags;
    r->width    = width;
    r->height   = height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    /* Request GL 4.5 Core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    r->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!r->window) { fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError()); return -2; }

    r->gl_ctx = SDL_GL_CreateContext(r->window);
    if (!r->gl_ctx) { fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError()); return -3; }
    SDL_GL_MakeCurrent(r->window, r->gl_ctx);
    SDL_GL_SetSwapInterval(1); /* vsync */

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        fprintf(stderr, "GLAD failed to load OpenGL functions\n");
        return -4;
    }

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&r->_gl_default_fbo);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* Built-in programs */
    r->_gl_prog_unlit_color = gl_make_program(VS_UNLIT_COLOR, FS_UNLIT_COLOR);
    r->_gl_prog_unlit_tex   = gl_make_program(VS_UNLIT_TEX,   FS_UNLIT_TEX);

    /* UBO for view/proj at binding 0 */
    glGenBuffers(1, &r->_gl_ubo_matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, r->_gl_ubo_matrices);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(struct UBO_Matrices), NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, r->_gl_ubo_matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    /* Default camera */
    r->camera.position = (vec_t){ .x=0, .y=5, .z=10 };
    r->camera.target   = (vec_t){ .x=0, .y=0, .z=0  };
    r->camera.up       = (vec_t){ .x=0, .y=1, .z=0  };
    r->camera.fov_deg  = 60.0; r->camera.nearp = 0.1; r->camera.farp = 200.0; r->camera.dim = 3;

    t_float64 aspect = (t_float64)width / (t_float64)height;
    r->camera.view = m4_lookat(r->camera.position, r->camera.target, r->camera.up, 3);
    r->camera.proj = m4_perspective(r->camera.fov_deg, aspect, r->camera.nearp, r->camera.farp);
    chaos_upload_vp_ubo(r);

    return 0;
}

void chaos_renderer_shutdown(chaos_renderer_t* r)
{
    if (!r) return;
    if (r->_gl_prog_unlit_color) glDeleteProgram(r->_gl_prog_unlit_color);
    if (r->_gl_prog_unlit_tex)   glDeleteProgram(r->_gl_prog_unlit_tex);
    if (r->_gl_ubo_matrices)     glDeleteBuffers(1, &r->_gl_ubo_matrices);
    if (r->gl_ctx)   { SDL_GL_DeleteContext(r->gl_ctx); r->gl_ctx = NULL; }
    if (r->window)   { SDL_DestroyWindow(r->window);    r->window = NULL; }
    SDL_Quit();
}

void chaos_renderer_resize(chaos_renderer_t* r, t_sint32 w, t_sint32 h)
{
    r->width = w; r->height = (h <= 0) ? 1 : h;
    glViewport(0, 0, r->width, r->height);
    t_float64 aspect = (t_float64)r->width / (t_float64)r->height;
    r->camera.proj = m4_perspective(r->camera.fov_deg, aspect, r->camera.nearp, r->camera.farp);
    chaos_upload_vp_ubo(r);
}


/* -------------------------------------------------------------------------- */
/* Frame                                                                      */
/* -------------------------------------------------------------------------- */

void chaos_begin_frame(chaos_renderer_t* r, t_float32 R, t_float32 G, t_float32 B, t_float32 A)
{
    glBindFramebuffer(GL_FRAMEBUFFER, r->_gl_default_fbo);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearColor(R, G, B, A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void chaos_end_frame(chaos_renderer_t* r)
{
    SDL_GL_SwapWindow(r->window);
}