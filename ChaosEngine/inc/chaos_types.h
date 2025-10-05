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
 * @file chaostype
 * @brief Portable custom type definitions for C programs.
 *
 * This header defines fixed-width integer types and a custom boolean type
 * for improved portability and code readability across different platforms.
 * It relies on the standard C header <stdint.h> for guaranteed integer sizes.
 *
 * @author PapaPamplemousse
 * @date October 2025
 * @version 1.0.0
 * @see stdint.h
 */

#ifndef CHAOS_TYPES_H
#define CHAOS_TYPES_H

/* ************************************************************************** */
/* INCLUDES                                                                   */
/* ************************************************************************** */

/**
 * @brief Includes standard C integer types with exact widths.
 *
 * This header provides definitions like uint32_t, int8_t, etc.,
 * which guarantee the size in bits (8, 16, 32, 64) regardless of the
 * compiler or the target architecture.
 */
#include <stdint.h>
/* For size_t (object size) and uintptr_t (integer capable of holding a pointer) */
#include <stddef.h> 

/* ************************************************************************** */
/* FIXED-WIDTH INTEGER TYPES (BASED ON <stdint.h>)                            */
/* ************************************************************************** */

/**
 * @name Fixed-Width Signed Integers
 * @{
 */
/** @typedef t_sint8
 * @brief 8-bit signed integer (1 byte).
 */
typedef int8_t      t_sint8;

/** @typedef t_sint16
 * @brief 16-bit signed integer (2 bytes).
 */
typedef int16_t     t_sint16;

/** @typedef t_sint32
 * @brief 32-bit signed integer (4 bytes).
 */
typedef int32_t     t_sint32;

/** @typedef t_sint64
 * @brief 64-bit signed integer (8 bytes).
 */
typedef int64_t     t_sint64;
/** @} */ /* End of Fixed-Width Signed Integers */


/**
 * @name Fixed-Width Unsigned Integers
 * @{
 */
/** @typedef t_uint8
 * @brief 8-bit unsigned integer (1 byte).
 */
typedef uint8_t     t_uint8;

/** @typedef t_uint16
 * @brief 16-bit unsigned integer (2 bytes).
 */
typedef uint16_t    t_uint16;

/** @typedef t_uint32
 * @brief 32-bit unsigned integer (4 bytes).
 */
typedef uint32_t    t_uint32;

/** @typedef t_uint64
 * @brief 64-bit unsigned integer (8 bytes).
 */
typedef uint64_t    t_uint64;
/** @} */ /* End of Fixed-Width Unsigned Integers */


/* ************************************************************************** */
/* BOOLEAN TYPE DEFINITION                                                    */
/* ************************************************************************** */

/**
 * @brief Custom boolean type definition using an enumeration.
 *
 * Defines a custom boolean type t_bool with associated constants TRUE and FALSE.
 * This convention provides a clear, type-safe boolean that is distinct from simple integers.
 */
typedef enum {
    FALSE = 0,    /**< Represents the logical FALSE value (integer 0). */
    TRUE = 1      /**< Represents the logical TRUE value (integer 1). */
} t_bool;

/* ************************************************************************** */
/* OTHER PRIMITIVE TYPES                                                      */
/* ************************************************************************** */

/**
 * @name Other Standard Types
 * @{
 */
/** @typedef t_char
 * @brief Standard character type (typically 1 byte).
 */
typedef char        t_char;

/** @typedef t_float32
 * @brief Single-precision floating-point number (typically 4 bytes, IEEE-754).
 */
typedef float       t_float32;

/** @typedef t_float64
 * @brief Double-precision floating-point number (typically 8 bytes, IEEE-754).
 */
typedef double      t_float64;

/** @typedef t_size
 * @brief Unsigned integer type returned by the sizeof operator.
 *
 * This type is guaranteed to be large enough to hold the size of the largest object
 * that can be declared on the system. Defined in <stddef.h>.
 */
typedef size_t      t_size;

/** @typedef t_uintptr
 * @brief Unsigned integer type capable of storing a data pointer.
 *
 * Useful for pointer arithmetic or converting pointers to integers safely. Defined in <stdint.h>.
 */
typedef uintptr_t   t_uintptr;
/** @} */ /* End of Other Standard Types */



/* ************* MATH TYPES ************* */
/**
 * @union vec_t
 * @brief Represents a vector in 2D or 3D space.
 * 
 * A union that allows accessing vector components either as named fields x, y, z
 * or as an array data[3]. The dimension in use is determined by the 'dim' field
 * in the body_t structure.
 */
typedef union {
    struct { t_float64 x, y, z; }; /**< @brief Access components by name (x, y, z). */
    t_float64 data[3];             /**< @brief Access components as an array. */
} vec_t;


/** 
 * @struct mat4
 * @brief Represents a 4x4 matrix, typically used for transformations (row-major). 
 * * The matrix is stored as a flat array of 16 double-precision floats.
 */
typedef struct 
{ 
    t_float64 m[16]; /**< @brief Matrix components (m[row*4 + col]). */
} mat4; /* row-major: m[row*4 + col] */

#endif /* TYPES_H */