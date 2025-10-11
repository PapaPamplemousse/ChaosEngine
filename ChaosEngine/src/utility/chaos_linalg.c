
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
 * @file chaos_linalg.c
 * @brief Minimal linear algebra implementation for ChaosEngine (no libc).
 */
#include "utility/chaos_linalg.h"
#include "utility/chaos_math.h"
#include "core/chaos_error.h"

/* FUNCTIONS */

ce_vec3_t ce_vec3_add(const ce_vec3_t a, const ce_vec3_t b)
{
    ce_vec3_t r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

ce_vec3_t ce_vec3_sub(const ce_vec3_t a, const ce_vec3_t b)
{
    ce_vec3_t r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

ce_vec3_t ce_vec3_scale(const ce_vec3_t a, ce_f32 s)
{
    ce_vec3_t r;
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    return r;
}

ce_f32 ce_vec3_dot(const ce_vec3_t a, const ce_vec3_t b)
{
    ce_f32 d = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    return d;
}

ce_vec3_t ce_vec3_cross(const ce_vec3_t a, const ce_vec3_t b)
{
    ce_vec3_t r;
    r.x = (a.y * b.z) - (a.z * b.y);
    r.y = (a.z * b.x) - (a.x * b.z);
    r.z = (a.x * b.y) - (a.y * b.x);
    return r;
}

ce_vec3_t ce_vec3_normalize(const ce_vec3_t a)
{
    ce_vec3_t r;
    ce_f32    len2 = (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
    if (len2 > 0.0f)
    {
        ce_f32 inv = 1.0f / ce_sqrt(len2);
        r.x        = a.x * inv;
        r.y        = a.y * inv;
        r.z        = a.z * inv;
    }
    else
    {
        r = a;
    }
    return r;
}

ce_mat4_t ce_mat4_identity(void)
{
    ce_mat4_t m;
    for (ce_u32 i = 0; i < 4; i++)
    {
        for (ce_u32 j = 0; j < 4; j++)
        {
            m.m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    return m;
}

ce_mat4_t ce_mat4_mul(const ce_mat4_t a, const ce_mat4_t b)
{
    ce_mat4_t r;
    for (ce_u32 i = 0; i < 4; i++)
    {
        for (ce_u32 j = 0; j < 4; j++)
        {
            ce_f32 sum = 0.0f;
            for (ce_u32 k = 0; k < 4; k++)
            {
                sum += a.m[i][k] * b.m[k][j];
            }
            r.m[i][j] = sum;
        }
    }
    return r;
}

ce_vec4_t ce_mat4_mul_vec4(const ce_mat4_t m, const ce_vec4_t v)
{
    ce_vec4_t r;
    r.x = (m.m[0][0] * v.x) + (m.m[0][1] * v.y) + (m.m[0][2] * v.z) + (m.m[0][3] * v.w);
    r.y = (m.m[1][0] * v.x) + (m.m[1][1] * v.y) + (m.m[1][2] * v.z) + (m.m[1][3] * v.w);
    r.z = (m.m[2][0] * v.x) + (m.m[2][1] * v.y) + (m.m[2][2] * v.z) + (m.m[2][3] * v.w);
    r.w = (m.m[3][0] * v.x) + (m.m[3][1] * v.y) + (m.m[3][2] * v.z) + (m.m[3][3] * v.w);
    return r;
}

ce_mat4_t ce_mat4_perspective(ce_f32 fov_rad, ce_f32 aspect, ce_f32 znear, ce_f32 zfar)
{
    ce_mat4_t m;
    for (ce_u32 i = 0; i < 4; i++)
    {
        for (ce_u32 j = 0; j < 4; j++)
        {
            m.m[i][j] = 0.0f;
        }
    }
    ce_f32 f      = 1.0f / ce_tan(fov_rad * 0.5f);
    ce_f32 inv_nf = 1.0f / (znear - zfar);
    m.m[0][0]     = f / aspect;
    m.m[1][1]     = f;
    m.m[2][2]     = (zfar + znear) * inv_nf;
    m.m[2][3]     = (2.0f * zfar * znear) * inv_nf;
    m.m[3][2]     = -1.0f;
    return m;
}

ce_mat4_t ce_mat4_look_at(const ce_vec3_t eye, const ce_vec3_t center, const ce_vec3_t up)
{
    ce_mat4_t m;
    for (ce_u32 i = 0; i < 4; i++)
    {
        for (ce_u32 j = 0; j < 4; j++)
        {
            m.m[i][j] = 0.0f;
        }
    }
    ce_vec3_t f = ce_vec3_normalize(ce_vec3_sub(center, eye));
    ce_vec3_t s = ce_vec3_normalize(ce_vec3_cross(f, up));
    ce_vec3_t u = ce_vec3_cross(s, f);
    m.m[0][0]   = s.x;
    m.m[0][1]   = s.y;
    m.m[0][2]   = s.z;
    m.m[0][3]   = -((s.x * eye.x) + (s.y * eye.y) + (s.z * eye.z));
    m.m[1][0]   = u.x;
    m.m[1][1]   = u.y;
    m.m[1][2]   = u.z;
    m.m[1][3]   = -((u.x * eye.x) + (u.y * eye.y) + (u.z * eye.z));
    m.m[2][0]   = -f.x;
    m.m[2][1]   = -f.y;
    m.m[2][2]   = -f.z;
    m.m[2][3]   = ((f.x * eye.x) + (f.y * eye.y) + (f.z * eye.z));
    m.m[3][3]   = 1.0f;
    return m;
}
