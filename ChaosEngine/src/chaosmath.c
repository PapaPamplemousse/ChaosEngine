#include <stdio.h>
#include "chaos_math.h"

/* ************* STATICS PROTOTYPES ************* */

static t_float64 reduce_angle(t_float64 x);

/* ************* FUNCTIONS ************* */

/**
 * @brief Calculates the power of a base raised to a non-negative integer exponent.
 *
 * @note This function is only reliable for non-negative integer exponents.
 *
 * @param base The base value (t_float64).
 * @param exp The integer exponent (t_sint32).
 * @return The result of base raised to the power of exp (t_float64).
 */
t_float64 chaos_pow(t_float64 base, t_sint32 exp) {
    if (exp == 0) return 1.0;
    
    // Handle negative exponents by inverting the base and using a positive exponent
    if (exp < 0) return 1.0 / chaos_pow(base, -exp);
    
    t_float64 res = 1.0;
    for (t_sint32 i = 0; i < exp; ++i) {
        res *= base;
    }
    return res;
}


/**
 * @brief Calculates the sine of an angle using the Maclaurin series.
 *
 * The series is: $\sin(x) = x - \frac{x^3}{3!} + \frac{x^5}{5!} - \dots$
 * The calculation stops when the magnitude of the current term is less than C_EPSILON.
 *
 * @param x The angle in radians (t_float64).
 * @return The sine of x (t_float64).
 */
t_float64 chaos_sin(t_float64 x) {
    x = reduce_angle(x); // Reduce angle for faster convergence
    
    t_float64 result = 0.0;
    t_float64 term = x; // First term: x^(2*0 + 1) / (2*0 + 1)! = x/1
    t_sint32 n = 1;
    
    // The loop continues as long as the term is significant
    while (term > C_EPSILON || term < -C_EPSILON) {
        result += term;
        n += 2; // Next term is for n+2
        
        // Optimized calculation for the next term:
        // term = -term * x^2 / (n * (n-1))
        term = -term * x * x / (n * (n - 1));
    }
    return result;
}

/**
 * @brief Calculates the cosine of an angle using the Maclaurin series.
 *
 * The series is: $\cos(x) = 1 - \frac{x^2}{2!} + \frac{x^4}{4!} - \dots$
 * The calculation stops when the magnitude of the current term is less than C_EPSILON.
 *
 * @param x The angle in radians (t_float64).
 * @return The cosine of x (t_float64).
 */
t_float64 chaos_cos(t_float64 x) {
    x = reduce_angle(x); // Reduce angle for faster convergence
    
    t_float64 result = 0.0;
    t_float64 term = 1.0; // First term: x^(2*0) / (2*0)! = 1
    t_sint32 n = 0;
    
    // The loop continues as long as the term is significant
    while (term > C_EPSILON || term < -C_EPSILON) {
        result += term;
        n += 2; // Next term is for n+2
        
        // Optimized calculation for the next term:
        // term = -term * x^2 / (n * (n-1))
        term = -term * x * x / (n * (n - 1));
    }
    return result;
}

/**
 * @brief Calculates the tangent of an angle ($\tan(x)$).
 *
 * Implemented using the identity: $\tan(x) = \frac{\sin(x)}{\cos(x)}$.
 * Includes a check for division by zero (asymptotes at $\pi/2 + n\pi$).
 *
 * @param x The angle in radians (t_float64).
 * @return The tangent of x (t_float64). Returns +/- Infinity if $\cos(x) \approx 0$.
 */
t_float64 chaos_tan(t_float64 x) {
    t_float64 cos_val = chaos_cos(x);
    
    // Check for values near zero (asymptotes)
    if (cos_val < C_EPSILON && cos_val > -C_EPSILON) {
        // Return +/- Infinity (or system-defined large number)
        // using 1.0/0.0 is the standard way to generate Infinity in C/IEEE 754
        return (x > 0) ? 1.0/0.0 : -1.0/0.0; 
    }
    return chaos_sin(x) / cos_val;
}

/**
 * @brief Calculates the square root of a non-negative number ($\sqrt{n}$)
 * using Heron's method (Newton's method for square roots).
 *
 * The iterative formula is: $x_{new} = \frac{1}{2} \left( x_{old} + \frac{n}{x_{old}} \right)$.
 *
 * @param n The non-negative number to find the square root of (t_float64).
 * @return The square root of n (t_float64). Returns NaN for negative inputs.
 */
t_float64 chaos_sqrt(t_float64 n) {
    if (n < 0.0) {
        fprintf(stderr, "Error: Square root of a negative number (NaN).\n");
        return 0.0 / 0.0; // NaN (Not a Number)
    }
    if (n == 0.0) return 0.0;

    t_float64 guess = n / 2.0; // Initial estimate
    t_float64 prev_guess;
    
    // Loop until the change between iterations is smaller than the tolerance
    do {
        prev_guess = guess;
        // Heron's Method formula
        guess = 0.5 * (prev_guess + n / prev_guess);
    // Check if the absolute difference is less than C_EPSILON
    } while ( (guess - prev_guess > C_EPSILON) || (prev_guess - guess > C_EPSILON) ); 
    
    return guess;
}

t_float64 chaos_atan2(t_float64 y, t_float64 x)
{
    if (x > 0.0)
    {
        return chaos_tan(y / x);
    }
    else if (x < 0.0 && y >= 0.0)
    {
        return chaos_tan(y / x) + C_PI;
    }
    else if (x < 0.0 && y < 0.0)
    {
        return chaos_tan(y / x) - C_PI;
    }
    else if (x == 0.0 && y > 0.0)
    {
        return C_PI / 2.0;
    }
    else if (x == 0.0 && y < 0.0)
    {
        return -C_PI / 2.0;
    }
    else
    {
        return 0.0;
    }
}


/* ************* STATIC FUNCTIONS ************* */

/**
 * @brief Reduces an angle to the primary range [-C_PI, C_PI].
 *
 * This step is crucial for the efficient and accurate convergence of the
 * Maclaurin/Taylor series for sine and cosine.
 *
 * @param x The angle in radians (t_float64).
 * @return The equivalent angle in the range (-C_PI, C_PI] (t_float64).
 */
static t_float64 reduce_angle(t_float64 x) {
    t_float64 two_pi = 2.0 * C_PI;
    
    // Calculate the remainder of the division by 2*C_PI
    // Casting to int (truncation) simulates the behavior of the fmod function
    // but without relying on math.h
    x = x - ( (t_sint32)(x / two_pi) ) * two_pi;
    
    // Adjust the angle to be within the [-C_PI, C_PI] range
    if (x > C_PI) {
        x -= two_pi;
    } else if (x <= -C_PI) {
        x += two_pi;
    }
    return x;
}

