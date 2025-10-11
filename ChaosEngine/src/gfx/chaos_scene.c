
#include "gfx/chaos_scene.h"
#include "core/chaos_error.h"
#include <SDL2/SDL.h>

static ce_vec3_t sph2cart(ce_f32 yaw, ce_f32 pitch, ce_f32 r)
{
    ce_vec3_t v;
    ce_f32    cp = ce_cos(pitch);
    v.x          = r * cp * ce_cos(yaw);
    v.y          = r * ce_sin(pitch);
    v.z          = r * cp * ce_sin(yaw);
    return v;
}

void ce_scene_init(ce_scene_t* s)
{
    if (s == 0)
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "scene_init");
    }
    else
    {
        s->cam.yaw      = 0.0f;
        s->cam.pitch    = 0.0f;
        s->cam.distance = 5.0f;
        s->cam.target   = (ce_vec3_t){0.0f, 0.0f, 0.0f};
        s->cam.fov_rad  = CHAOS_DEG2RAD(60.0f);
        s->cam.znear    = 0.01f;
        s->cam.zfar     = 1000.0f;
        s->mesh_count   = 0U;
        for (ce_u32 i = 0; i < CHAOS_SCENE_MAX_MESHES; i++)
        {
            s->meshes[i] = 0;
        }
    }
}

void ce_scene_add_mesh(ce_scene_t* s, ce_mesh_t* m)
{
    if ((s == 0) || (m == 0))
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "scene_add_mesh");
    }
    else
    {
        if (s->mesh_count >= CHAOS_SCENE_MAX_MESHES)
        {
            CE_RAISE_ERROR(CHAOS_ERR_INDEX_OUT_OF_BOUNDS, "scene full");
        }
        else
        {
            s->meshes[s->mesh_count] = m;
            s->mesh_count++;
        }
    }
}

void ce_camera_orbit(ce_camera_t* cam, ce_f32 delta_yaw, ce_f32 delta_pitch, ce_f32 delta_zoom)
{
    /* Compute zoom as exponential scaling for smoother feeling */
    ce_f32 zoom_factor = 0.0f;

    if (cam == (ce_camera_t*)0)
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "camera_orbit null");
    }
    else
    {
        /* Update rotation */
        cam->yaw += delta_yaw;
        cam->pitch += delta_pitch;

        /* Clamp vertical rotation (avoid flipping over the top) */
        cam->pitch = ce_clamp(cam->pitch, -CHAOS_DEG2RAD(89.0f), CHAOS_DEG2RAD(89.0f));
        if (delta_zoom != 0.0f)
        {
            zoom_factor = 1.0f + (delta_zoom * 0.1f);

            if (zoom_factor < 0.1f)
            {
                zoom_factor = 0.1f;
            }
            cam->distance *= zoom_factor;

            /* Clamp camera distance */
            if (cam->distance < 0.1f)
            {
                cam->distance = 0.1f;
            }
            if (cam->distance > 10000.0f)
            {
                cam->distance = 10000.0f;
            }
        }
    }
}

void ce_scene_render_software(ce_scene_t* s, SDL_Renderer* rnd, ce_u32 w, ce_u32 h)
{
    if ((s == 0) || (rnd == 0) || (w == 0U) || (h == 0U))
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "scene_render");
    }
    else
    {
        ce_vec3_t eye  = sph2cart(s->cam.yaw, s->cam.pitch, s->cam.distance);
        eye            = ce_vec3_add(eye, s->cam.target);
        ce_mat4_t view = ce_mat4_look_at(eye, s->cam.target, (ce_vec3_t){0.0f, 1.0f, 0.0f});
        ce_mat4_t proj = ce_mat4_perspective(s->cam.fov_rad, (ce_f32)w / (ce_f32)h, s->cam.znear, s->cam.zfar);
        ce_mat4_t vp   = ce_mat4_mul(proj, view);
        for (ce_u32 mi = 0; mi < s->mesh_count; ++mi)
        {
            ce_mesh_t* m = s->meshes[mi];
            if (m == 0)
            {
                continue;
            }
            for (ce_u32 i = 0; i + 2 < m->index_count; i += 3)
            {
                ce_u32    i0 = m->indices[i + 0], i1 = m->indices[i + 1], i2 = m->indices[i + 2];
                ce_u32    o0 = i0 * 3U, o1 = i1 * 3U, o2 = i2 * 3U;
                ce_vec4_t v0 = {m->vertices[o0 + 0], m->vertices[o0 + 1], m->vertices[o0 + 2], 1.0f};
                ce_vec4_t v1 = {m->vertices[o1 + 0], m->vertices[o1 + 1], m->vertices[o1 + 2], 1.0f};
                ce_vec4_t v2 = {m->vertices[o2 + 0], m->vertices[o2 + 1], m->vertices[o2 + 2], 1.0f};
                ce_vec4_t p0 = ce_mat4_mul_vec4(vp, v0);
                ce_vec4_t p1 = ce_mat4_mul_vec4(vp, v1);
                ce_vec4_t p2 = ce_mat4_mul_vec4(vp, v2);
                if ((p0.w != 0.0f) && (p1.w != 0.0f) && (p2.w != 0.0f))
                {
                    ce_f32 x0 = p0.x / p0.w, y0 = p0.y / p0.w;
                    ce_f32 x1 = p1.x / p1.w, y1 = p1.y / p1.w;
                    ce_f32 x2 = p2.x / p2.w, y2 = p2.y / p2.w;
                    int    sx0 = (int)((x0 * 0.5f + 0.5f) * (ce_f32)w);
                    int    sy0 = (int)((-y0 * 0.5f + 0.5f) * (ce_f32)h);
                    int    sx1 = (int)((x1 * 0.5f + 0.5f) * (ce_f32)w);
                    int    sy1 = (int)((-y1 * 0.5f + 0.5f) * (ce_f32)h);
                    int    sx2 = (int)((x2 * 0.5f + 0.5f) * (ce_f32)w);
                    int    sy2 = (int)((-y2 * 0.5f + 0.5f) * (ce_f32)h);
                    SDL_RenderDrawLine(rnd, sx0, sy0, sx1, sy1);
                    SDL_RenderDrawLine(rnd, sx1, sy1, sx2, sy2);
                    SDL_RenderDrawLine(rnd, sx2, sy2, sx0, sy0);
                }
            }
        }
    }
}

void ce_camera_fit_bounds(ce_camera_t* cam, const ce_bounds3_t* b)
{
    if ((cam == (ce_camera_t*)0) || (b == (const ce_bounds3_t*)0))
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "camera_fit_bounds invalid");
    }
    else
    {
        ce_f32 r    = b->radius;
        ce_f32 dist = 0.0f;
        ce_f32 f    = 0.0f;

        /* Guard zero radius */
        if (r < 1e-5f)
        {
            r = 1.0f;
        }

        /* distance estimate from FOV: dist ≈ r / sin(fov/2) */
        f = ce_sin(cam->fov_rad * 0.5f);
        if (f <= 0.0f)
        {
            f = 0.5f;
        }
        dist = (r / f) * 1.2f; /* add margin */

        cam->target   = b->center;
        cam->distance = dist;
        /* give a nice starting angle */
        cam->yaw   = CHAOS_DEG2RAD(45.0f);
        cam->pitch = CHAOS_DEG2RAD(-20.0f);
        if (cam->distance < (cam->znear * 10.0f))
        {
            cam->distance = cam->znear * 10.0f;
        }
    }
    return;
}

/**
 * @brief Translate (pan) the camera target in view plane.
 * @param cam Camera pointer.
 * @param delta_x Horizontal drag (pixels or arbitrary units).
 * @param delta_y Vertical drag.
 * @param speed Pan sensitivity factor (world units per screen unit).
 */
void ce_camera_pan(ce_camera_t* cam, ce_f32 delta_x, ce_f32 delta_y, ce_f32 speed)
{
    ce_vec3_t right;
    ce_vec3_t up;
    ce_vec3_t move;
    ce_f32    sin_yaw;
    ce_f32    cos_yaw;
    ce_f32    sin_pitch;
    ce_f32    cos_pitch;

    if (cam == (ce_camera_t*)0)
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "camera_pan null");
    }
    else
    {
        /* Precompute sin/cos (ce_math helpers, pas de <math.h>) */
        sin_yaw   = ce_sin(cam->yaw);
        cos_yaw   = ce_cos(cam->yaw);
        sin_pitch = ce_sin(cam->pitch);
        cos_pitch = ce_cos(cam->pitch);

        /* Camera basis vectors */
        right.x = cos_yaw;
        right.y = 0.0f;
        right.z = -sin_yaw;

        up.x = sin_yaw * sin_pitch;
        up.y = cos_pitch;
        up.z = cos_yaw * sin_pitch;

        /* Movement vector */
        move.x = (-delta_x * speed * right.x) + (delta_y * speed * up.x);
        move.y = (-delta_x * speed * right.y) + (delta_y * speed * up.y);
        move.z = (-delta_x * speed * right.z) + (delta_y * speed * up.z);

        /* Apply translation to target */
        cam->target.x += move.x;
        cam->target.y += move.y;
        cam->target.z += move.z;
    }

    return;
}
