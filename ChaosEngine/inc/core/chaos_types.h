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
 * @file chaos_types.h
 * @brief Fully self-contained ChaosEngine type definitions with guaranteed sizes.
 *
 * No dependency on libc or <stdint.h>.
 * Includes compile-time validation of type widths.
 */

#ifndef CHAOS_TYPES_H
#define CHAOS_TYPES_H

/* ************************************************************************** */
/* PLATFORM-INDEPENDENT INTEGER TYPES                                         */
/* ************************************************************************** */

/* Signed integers */
typedef signed char      ce_s8;
typedef signed short     ce_s16;
typedef signed int       ce_s32;
typedef signed long long ce_s64;

/* Unsigned integers */
typedef unsigned char      ce_u8;
typedef unsigned short     ce_u16;
typedef unsigned int       ce_u32;
typedef unsigned long long ce_u64;

/* ************************************************************************** */
/* COMPILE-TIME SIZE VALIDATION                                               */
/* ************************************************************************** */
#define CE_STATIC_ASSERT(expr, msg) typedef char static_assert_##msg[(expr) ? 1 : -1]

CE_STATIC_ASSERT(sizeof(ce_s8) == 1, s8_must_be_1_byte);
CE_STATIC_ASSERT(sizeof(ce_s16) == 2, s16_must_be_2_bytes);
CE_STATIC_ASSERT(sizeof(ce_s32) == 4, s32_must_be_4_bytes);
CE_STATIC_ASSERT(sizeof(ce_s64) == 8, s64_must_be_8_bytes);

CE_STATIC_ASSERT(sizeof(ce_u8) == 1, u8_must_be_1_byte);
CE_STATIC_ASSERT(sizeof(ce_u16) == 2, u16_must_be_2_bytes);
CE_STATIC_ASSERT(sizeof(ce_u32) == 4, u32_must_be_4_bytes);
CE_STATIC_ASSERT(sizeof(ce_u64) == 8, u64_must_be_8_bytes);

/* ************************************************************************** */
/* BOOLEAN TYPE                                                               */
/* ************************************************************************** */

typedef enum ce_bool_e
{
    CE_FALSE = 0,
    CE_TRUE  = 1
} ce_bool;

/* ************************************************************************** */
/* FLOATING-POINT TYPES                                                       */
/* ************************************************************************** */
typedef float  ce_f32;
typedef double ce_f64;

CE_STATIC_ASSERT(sizeof(ce_f32) == 4, f32_must_be_4_bytes);
CE_STATIC_ASSERT(sizeof(ce_f64) == 8, f64_must_be_8_bytes);

/* ************************************************************************** */
/* SIZE & POINTER TYPES                                                       */
/* ************************************************************************** */

/**
 * @brief Integer type large enough to represent a pointer.
 *
 * On 32-bit builds: 4 bytes
 * On 64-bit builds: 8 bytes
 */
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)
typedef ce_u64 ce_uptr;
#else
typedef ce_u32 ce_uptr;
#endif

/**
 * @brief Type large enough to represent object sizes.
 */
typedef ce_uptr ce_size;

/* ************************************************************************** */
/* CHARACTER TYPE                                                             */
/* ************************************************************************** */
typedef char ce_char;

/* ************************************************************************** */
/* MACROS                                                                     */
/* ************************************************************************** */
#define CE_ARRAY_COUNT(x) ((ce_size)(sizeof(x) / sizeof((x)[0])))
#define CE_ALIGN_UP(v, a) (((v) + ((a) - 1)) & ~((a) - 1))
#define CE_ALIGN_DOWN(v, a) ((v) & ~((a) - 1))

#endif /* CHAOS_TYPES_H */
