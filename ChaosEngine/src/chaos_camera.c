#include "chaos_camera.h"
#include "chaos_renderer_core.h"
#include "chaos_linalg.h"

#include <glad/gl.h>
#include <math.h>



/* -------------------------------------------------------------------------- */
/* CAMERA API                                                                 */
/* -------------------------------------------------------------------------- */
void chaos_camera_setup(chaos_renderer_t* r,
                        vec_t eye, vec_t target, vec_t up,
                        t_float64 fov_deg, t_float64 nearp, t_float64 farp)
{
    r->camera.position = eye; r->camera.target = target; r->camera.up = up;
    r->camera.fov_deg = fov_deg; r->camera.nearp = nearp; r->camera.farp = farp; r->camera.dim = 3;
    r->camera.view = m4_lookat(eye, target, up, 3);
    t_float64 aspect = (t_float64)r->width / (t_float64)r->height;
    r->camera.proj = m4_perspective(fov_deg, aspect, nearp, farp);
    chaos_upload_vp_ubo(r);
}

void chaos_camera_reset(chaos_renderer_t* r)
{
    r->camera.position = vec3(6.0, 6.0, 10.0);
    r->camera.target   = vec3(0.0, 0.0, 0.0);
    r->camera.up       = vec3(0.0, 1.0, 0.0);
    r->camera.dim      = 3;
    r->camera.fov_deg  = 60.0;
    r->camera.nearp    = 0.1;
    r->camera.farp     = 200.0;

    chaos_camera_setup(r, r->camera.position, r->camera.target, r->camera.up,
                       r->camera.fov_deg, r->camera.nearp, r->camera.farp);
}

void chaos_camera_zoom(chaos_renderer_t* r, t_float32 delta)
{
    vec_t dir = vec_norm(vec_sub(r->camera.target, r->camera.position, 3), 3);
    vec_t newEye = vec_add(r->camera.position, vec_scale(dir, delta, 3), 3);
    r->camera.position = newEye;

    chaos_camera_setup(r, r->camera.position, r->camera.target, r->camera.up,
                       r->camera.fov_deg, r->camera.nearp, r->camera.farp);
}

void chaos_camera_orbit(chaos_renderer_t* r, t_float32 dYaw, t_float32 dPitch)
{
    /* Convert degrees to radians */
    t_float64 yaw = dYaw * (C_PI / 180.0);
    t_float64 pitch = dPitch * (C_PI / 180.0);

    /* Vector from target to camera */
    vec_t offset = vec_sub(r->camera.position, r->camera.target, 3);

    /* Spherical coordinates */
    t_float64 radius = chaos_sqrt(vec_dot(offset, offset));
    t_float64 theta = chaos_atan2(offset.z, offset.x);
    t_float64 phi   = chaos_atan2(offset.y, chaos_sqrt(offset.x*offset.x + offset.z*offset.z));

    theta += yaw;
    phi   += pitch;
    if (phi > 1.5) phi = 1.5;
    if (phi < -1.5) phi = -1.5;

    offset.x = radius * chaos_cos(phi) * chaos_cos(theta);
    offset.z = radius * chaos_cos(phi) * chaos_sin(theta);
    offset.y = radius * chaos_sin(phi);

    r->camera.position = vec_add(r->camera.target, offset, 3);

    chaos_camera_setup(r, r->camera.position, r->camera.target, r->camera.up,
                       r->camera.fov_deg, r->camera.nearp, r->camera.farp);
}

t_float32 chaos_compute_point_scale(const chaos_renderer_t* r)
{
    /* pointScale = viewport_height / (2 * tan(fov/2)) */
    const t_float64 fov_rad = r->camera.fov_deg * (C_PI / 180.0);
    return (t_float32)((t_float64)r->height / (2.0 * chaos_tan(fov_rad * 0.5)));
}

