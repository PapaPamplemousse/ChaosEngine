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

#include "chaos_types.h" 


/* ************* CONSTANTS ************* */

/**
 * @brief Approximation of Pi (mathematical constant).
 */
#define C_PI 3.14159265358979323846

/**
 * @brief Tolerance for Taylor series convergence and iterative methods.
 *
 * The series will stop when the magnitude of a term falls below this value.
 */
#define C_EPSILON 1e-9

/* ************* FUNCTIONS ************* */


/**
 * @brief Absolute value for double-precision floats.
 */
static inline t_float64 chaos_abs(t_float64 x) {
    return (x < 0.0) ? -x : x;
}

/**
 * @brief Absolute value for single-precision floats.
 */
static inline t_float32 chaos_absf(t_float32 x) {
    return (x < 0.0f) ? -x : x;
}

/**
 * @brief Absolute value for signed 32-bit integers.
 */
static inline t_sint32 chaos_absi(t_sint32 x) {
    return (x < 0) ? -x : x;
}

/**
 * @brief Calculates the power of a base raised to an integer exponent (base^exp).
 *
 * @param base The base value (t_float64).
 * @param exp The integer exponent (t_sint32).
 * @return The result of base raised to the power of exp (t_float64).
 */
extern t_float64 chaos_pow(t_float64 base, t_sint32 exp);

/**
 * @brief Calculates the sine of an angle using the Maclaurin series.
 *
 * @param x The angle in radians (t_float64).
 * @return The sine of x (t_float64).
 */
extern t_float64 chaos_sin(t_float64 x);

/**
 * @brief Calculates the cosine of an angle using the Maclaurin series.
 *
 * @param x The angle in radians (t_float64).
 * @return The cosine of x (t_float64).
 */
extern t_float64 chaos_cos(t_float64 x);

/**
 * @brief Calculates the tangent of an angle ($\tan(x)$).
 *
 * @param x The angle in radians (t_float64).
 * @return The tangent of x (t_float64). Returns +/- Infinity if $\cos(x) \approx 0$.
 */
extern t_float64 chaos_tan(t_float64 x);

/**
 * @brief Calculates the square root of a non-negative number ($\sqrt{n}$)
 * using Heron's method.
 *
 * @param n The non-negative number to find the square root of (t_float64).
 * @return The square root of n (t_float64). Returns NaN for negative inputs.
 */
extern t_float64 chaos_sqrt(t_float64 n);


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
extern t_float64 chaos_atan2(t_float64 y, t_float64 x);

#endif