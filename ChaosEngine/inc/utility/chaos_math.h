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
 * @file chaos_math.h
 * @brief Implementation of fundamental mathematical functions (sin, cos, tan, sqrt)
 * using numerical methods without relying on the standard C math.h library.
 *
 * This file implements trigonometric and algebraic functions using Taylor series
 * (for sin/cos) and the Heron's method (for sqrt).
 *
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0.0
 */
#ifndef CHAOS_MATH_H
#define CHAOS_MATH_H

#include "core/chaos_types.h"

/* ************* CONSTANTS ************* */

/**
 * @brief Approximation of Pi (mathematical constant).
 */
#define CHAOS_PI_F32 (3.14159265358979323846f)

/**
 * @brief Tolerance for Taylor series convergence and iterative methods.
 *
 * The series will stop when the magnitude of a term falls below this value.
 */
#define C_EPSILON 1e-9

/* ************* MACROS DEFINITIONS ************* */

/**
 * @brief Converts degrees to radians.
 */
#define CHAOS_DEG2RAD(x) ((ce_f32)((x) * (CHAOS_PI_F32 / 180.0f)))

/**
 * @brief Converts radians to degrees.
 */
#define CHAOS_RAD2DEG(x) ((ce_f32)((x) * (180.0f / CHAOS_PI_F32)))

/* ************* INLINE HELPERS ************* */

/**
 * @brief Absolute value for double-precision floats.
 */
static inline ce_f32 ce_abs(ce_f32 x)
{
    return (x < 0.0) ? -x : x;
}

/**
 * @brief Absolute value for single-precision floats.
 */
static inline ce_f32 ce_absf(ce_f32 x)
{
    return (x < 0.0f) ? -x : x;
}

/**
 * @brief Absolute value for signed 32-bit integers.
 */
static inline ce_s32 ce_absi(ce_s32 x)
{
    return (x < 0) ? -x : x;
}

/* ************* FUNCTION DECLARATIONS ************* */

/**
 * @brief Calculates the power of a base raised to an integer exponent (base^exp).
 *
 * @param base The base value (ce_f32).
 * @param exp The integer exponent (ce_s32).
 * @return The result of base raised to the power of exp (ce_f32).
 */
extern ce_f32 ce_pow(ce_f32 base, ce_s32 exp);

/**
 * @brief Calculates the sine of an angle using the Maclaurin series.
 *
 * @param x The angle in radians (ce_f32).
 * @return The sine of x (ce_f32).
 */
extern ce_f32 ce_sin(ce_f32 x);

/**
 * @brief Calculates the cosine of an angle using the Maclaurin series.
 *
 * @param x The angle in radians (ce_f32).
 * @return The cosine of x (ce_f32).
 */
extern ce_f32 ce_cos(ce_f32 x);

/**
 * @brief Calculates the tangent of an angle ($\tan(x)$).
 *
 * @param x The angle in radians (ce_f32).
 * @return The tangent of x (ce_f32). Returns +/- Infinity if $\cos(x) \approx 0$.
 */
extern ce_f32 ce_tan(ce_f32 x);

/**
 * @brief Calculates the square root of a non-negative number ($\sqrt{n}$)
 * using Heron's method.
 *
 * @param x The non-negative number to find the square root of (ce_f32).
 * @return The square root of n (ce_f32). Returns NaN for negative inputs.
 */
extern ce_f32 ce_sqrt(ce_f32 x);

/**
 * @brief Computes the arc-tangent of y/x, accounting for the quadrant.
 *
 * This implementation avoids using math.h and replicates the behavior of atan2(y, x).
 * It returns the angle in radians between -π and +π.
 *
 * @param y The ordinate component.
 * @param x The abscissa component.
 * @return The angle θ = atan2(y, x), in radians.
 */
extern ce_f32 ce_atan2(ce_f32 y, ce_f32 x);

/**
 * @brief Clamps a floating-point value to a specified range [lo, hi].
 *
 * @param v The value to clamp (ce_f32).
 * @param lo The lower bound of the range (ce_f32).
 * @param hi The upper bound of the range (ce_f32).
 * @return The clamped value (ce_f32).
 */
extern ce_f32 ce_clamp(ce_f32 v, ce_f32 lo, ce_f32 hi);

#endif
