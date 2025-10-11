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
 * @file chaos_math.c
 * @brief Implementation of fundamental mathematical functions (sin, cos, tan, sqrt)
 * using numerical methods without relying on the standard C math.h library.
 */

#include "utility/chaos_math.h"
#include "core/chaos_error.h" /* For CE_RAISE_ERROR */
/* ************* STATICS PROTOTYPES ************* */

static ce_f32 reduce_angle(ce_f32 x);

/* ************* FUNCTIONS ************* */

ce_f32 ce_pow(ce_f32 base, ce_s32 exp)
{
    ce_f32  result;
    ce_s32  i;
    ce_bool invert;

    result = 1.0;
    invert = CE_FALSE;

    if (exp < 0)
    {
        invert = CE_TRUE;
        exp    = -exp;
    }
    else
    {
        if (exp == 0)
        {
            /* nothing to do, result = 1 */
        }
    }

    for (i = 0; i < exp; i++)
    {
        result *= base;
    }

    if (invert == CE_TRUE)
    {
        result = 1.0 / result;
    }

    return result;
}

ce_f32 ce_sin(ce_f32 x)
{
    x             = reduce_angle(x); // Reduce angle for faster convergence
    ce_f32 result = 0.0;
    ce_f32 term   = x; // First term: x^(2*0 + 1) / (2*0 + 1)! = x/1
    ce_s32 n      = 1;

    // The loop continues as long as the term is significant
    while (term > C_EPSILON || term < -C_EPSILON)
    {
        result += term;
        n += 2; // Next term is for n+2

        // Optimized calculation for the next term:
        // term = -term * x^2 / (n * (n-1))
        term = -term * x * x / (n * (n - 1));
    }
    return result;
}

ce_f32 ce_cos(ce_f32 x)
{
    x = reduce_angle(x); // Reduce angle for faster convergence

    ce_f32 result = 0.0;
    ce_f32 term   = 1.0; // First term: x^(2*0) / (2*0)! = 1
    ce_s32 n      = 0;

    // The loop continues as long as the term is significant
    while (term > C_EPSILON || term < -C_EPSILON)
    {
        result += term;
        n += 2; // Next term is for n+2

        // Optimized calculation for the next term:
        // term = -term * x^2 / (n * (n-1))
        term = -term * x * x / (n * (n - 1));
    }
    return result;
}

ce_f32 ce_tan(ce_f32 x)
{
    ce_f32 result;
    ce_f32 cos_val;
    ce_f32 sin_val;

    result  = 0.0;
    cos_val = ce_cos(x);

    if ((cos_val < C_EPSILON) && (cos_val > -C_EPSILON))
    {
        CE_RAISE_ERROR(CHAOS_ERR_MATH_DIV_BY_ZERO, "ce_tan: division by zero (cos=0)");
        if (x > 0.0)
        {
            result = 1.0 / 0.0; /* +Inf */
        }
        else
        {
            result = -1.0 / 0.0; /* -Inf */
        }
    }
    else
    {
        sin_val = ce_sin(x);
        result  = sin_val / cos_val;
    }

    return result;
}

ce_f32 ce_sqrt(ce_f32 x)
{
    ce_f32 y = 0.0f;
    ce_u32 i = 0U;

    if (x <= 0.0f)
    {
        y = 0.0f;
    }
    else
    {
        y = x; /* initial */
        for (i = 0U; i < 3U; ++i)
        {
            /* y = 0.5f * (y + x / y) */
            if (y != 0.0f)
            {
                y = 0.5f * (y + (x / y));
            }
        }
    }
    return y;
}

ce_f32 ce_atan2(ce_f32 y, ce_f32 x)
{
    ce_f32 result;
    ce_f32 ratio;

    result = 0.0;

    if (x > 0.0)
    {
        ratio  = y / x;
        result = ce_tan(ratio);
    }
    else if ((x < 0.0) && (y >= 0.0))
    {
        ratio  = y / x;
        result = ce_tan(ratio) + CHAOS_PI_F32;
    }
    else if ((x < 0.0) && (y < 0.0))
    {
        ratio  = y / x;
        result = ce_tan(ratio) - CHAOS_PI_F32;
    }
    else if ((x == 0.0) && (y > 0.0))
    {
        result = CHAOS_PI_F32 / 2.0;
    }
    else if ((x == 0.0) && (y < 0.0))
    {
        result = -CHAOS_PI_F32 / 2.0;
    }
    else
    {
        /* (x == 0 && y == 0) → undefined direction */
        CE_RAISE_WARN(CHAOS_WARN_UNINITIALIZED_VALUE, "ce_atan2: undefined angle (x=y=0)");
    }

    return result;
}

ce_f32 ce_clamp(ce_f32 v, ce_f32 lo, ce_f32 hi)
{
    ce_f32 out = v;
    if (out < lo)
    {
        out = lo;
    }
    if (out > hi)
    {
        out = hi;
    }
    return out;
}

/* ************* STATIC FUNCTIONS ************* */

/**
 * @brief Reduces an angle to the primary range [-CHAOS_PI_F32, CHAOS_PI_F32].
 *
 * This step is crucial for the efficient and accurate convergence of the
 * Maclaurin/Taylor series for sine and cosine.
 *
 * @param x The angle in radians (ce_f32).
 * @return The equivalent angle in the range (-CHAOS_PI_F32, CHAOS_PI_F32] (ce_f32).
 */
static ce_f32 reduce_angle(ce_f32 x)
{
    ce_f32 two_pi = 2.0 * CHAOS_PI_F32;

    // Calculate the remainder of the division by 2*CHAOS_PI_F32
    // Casting to int (truncation) simulates the behavior of the fmod function
    // but without relying on math.h
    x = x - ((ce_s32)(x / two_pi)) * two_pi;

    // Adjust the angle to be within the [-CHAOS_PI_F32, CHAOS_PI_F32] range
    if (x > CHAOS_PI_F32)
    {
        x -= two_pi;
    }
    else if (x <= -CHAOS_PI_F32)
    {
        x += two_pi;
    }
    return x;
}
