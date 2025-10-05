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
 * @file chaos_linalg.h
 * @brief Structures and operations for **Linear Algebra** (Vectors and Matrices) in ChaosEngine.
 *
 * This header defines the core structures like **vec_t** (3D Vector) and **mat4** (4x4 Matrix)
 * and provides inline functions for fundamental **vector operations** (addition, scaling, dot, cross,
 * normalization) and **matrix operations** (multiplication, identity, perspective, look-at,
 * point transformation). It is the foundation for 3D graphics and physics transformations.
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0
 * */
#ifndef CHAOS_LINALG_H 
#define CHAOS_LINALG_H

#include "chaos_types.h"
#include "chaos_math.h"



/* ************* INLINES ************* */
/**
 * @brief Creates a 2D vector with z=0.
 *
 * @param x X component
 * @param y Y component
 * @return vec_t Vector with x, y and z=0
 */
static inline vec_t vec2(t_float64 x, t_float64 y) {
    return (vec_t){ .x = x, .y = y, .z = 0.0 };
}

/**
 * @brief Creates a 3D vector.
 *
 * @param x X component
 * @param y Y component
 * @param z Z component
 * @return vec_t Vector with x, y, z
 */
static inline vec_t vec3(t_float64 x, t_float64 y, t_float64 z) {
    return (vec_t){ .x = x, .y = y, .z = z };
}

/**
 * @brief Adds two vectors together component-wise.
 * 
 * @param a First vector.
 * @param b Second vector.
 * @param dim Dimension to use (2 or 3).
 * @return vec_t Resulting vector (a + b).
 */
static inline vec_t vec_add(vec_t a, vec_t b, t_uint8 dim) {
    vec_t r;
    for (t_uint8 i = 0; i < dim; ++i) r.data[i] = a.data[i] + b.data[i];
    return r;
}

/**
 * @brief Substract two vectors together component-wise.
 * 
 * @param a First vector.
 * @param b Second vector.
 * @param dim Dimension to use (2 or 3).
 * @return vec_t Resulting vector (a + b).
 */
static inline vec_t vec_sub(vec_t a, vec_t b, t_uint8 dim) {
    vec_t r;
    for (t_uint8 i = 0; i < dim; ++i)
        r.data[i] = a.data[i] - b.data[i];
    return r;
}

/**
 * @brief Scales a vector by a scalar.
 * 
 * @param a Vector to scale.
 * @param s Scalar multiplier.
 * @param dim Dimension to use (2 or 3).
 * @return vec_t Resulting scaled vector.
 */
static inline vec_t vec_scale(vec_t a, t_float64 s, t_uint8 dim) {
    vec_t r;
    for (t_uint8 i = 0; i < dim; ++i) r.data[i] = a.data[i]*s;
    return r;
}

/**
 * @brief Computes the dot product (scalar product) of two vectors.
 * * @param a First vector.
 * @param b Second vector.
 * @return t_float64 The dot product $a \cdot b$.
 */
static inline t_float64 vec_dot(vec_t a, vec_t b) { return a.x*b.x + a.y*b.y + a.z*b.z; }

/**
 * @brief Computes the cross product (vector product) of two vectors.
 * * @note This operation is only meaningful in 3D space.
 * @param a First vector.
 * @param b Second vector.
 * @return vec_t The resulting vector $\text{a} \times \text{b}$, orthogonal to both inputs.
 */
static inline vec_t vec_cross(vec_t a, vec_t b) {
    return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

/**
 * @brief Computes the unit vector (normalization) of a vector.
 * @param a The input vector.
 * @param dim Dimension 2D / 3D
 * @return vec_t The normalized vector with length 1. Returns a zero vector if the input vector has zero length.
 */
static inline vec_t vec_norm(vec_t a, t_uint8 dim) {
    t_float64 l = chaos_sqrt(vec_dot(a, a));
    return (l > 0.0) ? vec_scale(a, 1.0 / l, dim) : vec3(0.0, 0.0, 0.0);
}


/**
 * @brief Create a 4x4 identity matrix
 */
static inline mat4 m4_identity(void) {
    mat4 M = {0};
    M.m[0] = M.m[5] = M.m[10] = M.m[15] = 1.0;
    return M;
}

/**
 * @brief Multiply two 4x4 matrix
 */
static inline mat4 m4_mul(mat4 A, mat4 B) {
    mat4 R = {0};
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            R.m[r*4 + c] =
                A.m[r*4 + 0]*B.m[0*4 + c] +
                A.m[r*4 + 1]*B.m[1*4 + c] +
                A.m[r*4 + 2]*B.m[2*4 + c] +
                A.m[r*4 + 3]*B.m[3*4 + c];
        }
    }
    return R;
}
/**
 * @brief Creates a 4x4 perspective projection matrix.
 *
 * This matrix transforms view-space coordinates into clip-space coordinates 
 * for 3D rendering.
 * @param fov_deg The field of view in degrees.
 * @param aspect The aspect ratio of the viewport (width / height).
 * @param nearp The distance to the near clipping plane.
 * @param farp The distance to the far clipping plane.
 * @return mat4 The perspective projection matrix P.
 */
static inline mat4 m4_perspective(t_float64 fov_deg, t_float64 aspect, t_float64 nearp, t_float64 farp) {
    t_float64 fov_rad = fov_deg * C_PI / 180.0;
    t_float64 t = 1.0 / chaos_tan(fov_rad * 0.5);
    mat4 P = {0};

    // P.m[row * 4 + col
    P.m[0]  = t / aspect;
    P.m[5]  = t;
    P.m[10] = (farp + nearp) / (nearp - farp);
    P.m[11] = -1.0;
    P.m[14] = (2.0 * farp * nearp) / (nearp - farp);
    return P;
}

/**
 * @brief Creates a 4x4 view matrix that positions and orientates the camera.
 * * This matrix transforms world-space coordinates into view-space coordinates.
 * * @note This function relies on vec_norm, vec_cross, vec_sub, vec_dot, and m4_identity being available.
 * @param eye The position of the camera (eye point).
 * @param center The point the camera is looking at.
 * @param up The vector representing the camera's up direction (must not be parallel to the view direction).
 * @param dim 2D / 3D
 * @return mat4 The view matrix V.
 */
static inline mat4 m4_lookat(vec_t eye, vec_t center, vec_t up, t_uint8 dim) {
    vec_t f = vec_norm(vec_sub(center, eye, dim), dim);
    vec_t s = vec_norm(vec_cross(f, up), dim);
    vec_t u = vec_cross(s, f);

    mat4 V = m4_identity();

    V.m[0] = s.x;  V.m[1] = s.y;  V.m[2]  = s.z;  V.m[3]  = -vec_dot(s, eye);
    V.m[4] = u.x;  V.m[5] = u.y;  V.m[6]  = u.z;  V.m[7]  = -vec_dot(u, eye);
    V.m[8] = -f.x; V.m[9] = -f.y; V.m[10] = -f.z; V.m[11] =  vec_dot(f, eye);
    V.m[15] = 1.0;
    return V;
}

/**
 * @brief Transforms a point from 3D space (represented as 4D: x,y,z,1) by a 4x4 matrix.
 * * Performs the matrix multiplication $\mathbf{P}' = \mathbf{M} \cdot \mathbf{P}$.
 * The output (x, y, z, w) is typically the resulting clip-space coordinate, which
 * requires perspective division (dividing x, y, z by w) afterwards.
 * * @param M The 4x4 transformation matrix.
 * @param p The 3D point (with implicit W=1).
 * @param x Output pointer for the transformed X coordinate.
 * @param y Output pointer for the transformed Y coordinate.
 * @param z Output pointer for the transformed Z coordinate.
 * @param w Output pointer for the transformed W coordinate (for perspective division).
 */
static inline void m4_mul_point(mat4 M, vec_t p, t_float64 *x, t_float64 *y, t_float64 *z, t_float64 *w) {
    // Row 1
    *x = M.m[0]*p.x + M.m[1]*p.y + M.m[2]*p.z + M.m[3]*1.0;
    // Row 2
    *y = M.m[4]*p.x + M.m[5]*p.y + M.m[6]*p.z + M.m[7]*1.0;
    // Row 3
    *z = M.m[8]*p.x + M.m[9]*p.y + M.m[10]*p.z + M.m[11]*1.0;
    // Row 4 (W component)
    *w = M.m[12]*p.x + M.m[13]*p.y + M.m[14]*p.z + M.m[15]*1.0;
}

/**
 * @brief Creates a 4x4 translation matrix from a vector.
 *
 * @param v Translation vector (x, y, z)
 * @return mat4 Translation matrix
 */
static inline mat4 m4_translate(vec_t v)
{
    mat4 M = m4_identity();
    M.m[3]  = v.x;
    M.m[7]  = v.y;
    M.m[11] = v.z;
    return M;
}

/**
 * @brief Creates a 4x4 uniform scaling matrix.
 *
 * @param s Scaling factor
 * @return mat4 Scaling matrix
 */
static inline mat4 m4_scale(t_float64 s)
{
    mat4 M = {0};
    M.m[0] = M.m[5] = M.m[10] = s;
    M.m[15] = 1.0;
    return M;
}

/**
 * @brief Creates a rotation matrix around the X axis.
 *
 * @param angle Angle in radians
 * @return mat4 Rotation matrix
 */
static inline mat4 m4_rotate_x(t_float64 angle)
{
    t_float64 c = chaos_cos(angle);
    t_float64 s = chaos_sin(angle);
    mat4 M = m4_identity();
    M.m[5] = c;  M.m[6]  = -s;
    M.m[9] = s;  M.m[10] = c;
    return M;
}

/**
 * @brief Creates a rotation matrix around the Y axis.
 *
 * @param angle Angle in radians
 * @return mat4 Rotation matrix
 */
static inline mat4 m4_rotate_y(t_float64 angle)
{
    t_float64 c = chaos_cos(angle);
    t_float64 s = chaos_sin(angle);
    mat4 M = m4_identity();
    M.m[0] = c;  M.m[2] = s;
    M.m[8] = -s; M.m[10] = c;
    return M;
}

/**
 * @brief Creates a rotation matrix around the Z axis.
 *
 * @param angle Angle in radians
 * @return mat4 Rotation matrix
 */
static inline mat4 m4_rotate_z(t_float64 angle)
{
    t_float64 c = chaos_cos(angle);
    t_float64 s = chaos_sin(angle);
    mat4 M = m4_identity();
    M.m[0] = c;  M.m[1] = -s;
    M.m[4] = s;  M.m[5] = c;
    return M;
}

/**
 * @brief Transform matrix^4 to float32
 *
 * @param mat4 
 * @param out16
 * @return mat4 Rotation matrix
 */
static inline void m4_to_f32(const mat4* M, t_float32 out16[16]) {
    for (int i = 0; i < 16; ++i) out16[i] = (t_float32)M->m[i];
}
#endif 
