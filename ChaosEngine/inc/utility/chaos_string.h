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
 * @file chaos_string.h
 * @brief Chaos-style string.h subset (no libc)
 * @author PapaPamplemousse
 */
#ifndef CHAOS_STRING_H
#define CHAOS_STRING_H

#include "core/chaos_types.h"


/**
* @brief Copies exactly n bytes from src to dst (non-overlapping expected, like standard memcpy).
* @param dst Destination pointer.
* @param src Source pointer.
* @param n Number of bytes to copy.
* @return Pointer to dst on success, CE_NULL (NULL) on failure.
* @note Defensive: returns CE_NULL if dst or src is NULL and n>0.
 */   
static inline void* ce__memcpy(void* dst, const void* src, ce_size n)
{
    void* ret;
    ce_u8* dptr;
    const ce_u8* sptr;
    ce_size i;

    ret  = dst;
    dptr = (ce_u8*)dst;
    sptr = (const ce_u8*)src;
    i    = 0;

    /* Defensive checks */
    if ((dst == (void*)0) || (src == (const void*)0)) {
        if (n != (ce_size)0) {
            ret = (void*)0;
        }
    } else {
        /* Undefined behavior on overlap in memcpy: we keep forward copy for perf parity. */
        while (i < n) {
            dptr[i] = sptr[i];
            i++;
        }
    }

    return ret;
}

/**
* @brief Fills dst with byte value 'value' for n bytes.
* @param dst Destination pointer.
* @param value Byte value to set (interpreted as ce_u8).
* @param n Number of bytes to set.
* @return Pointer to dst on success, CE_NULL (NULL) on failure.
* @note Defensive: returns CE_NULL if dst is NULL and n>0.
*/  
static inline void* ce__memset(void* dst, ce_u8 value, ce_size n)
{
    void* ret;
    ce_u8* dptr;
    ce_size i;

    ret  = dst;
    dptr = (ce_u8*)dst;
    i    = 0;

    if (dst == (void*)0) {
        if (n != (ce_size)0) {
            ret = (void*)0;
        }
    } else {
        while (i < n) {
            dptr[i] = value;
            i++;
        }
    }

    return ret;
}

/**
 * @brief Lexicographical compare of two byte ranges a and b over n bytes.
 * @param a First memory block pointer.
 * @param b Second memory block pointer.
 * @param n Number of bytes to compare.
 * @return <0 if a<b, 0 if equal, >0 if a>b; returns 0 if a or b is NULL and n>0 (defensive).
 * @note Single return policy.
 */
static inline ce_s32 ce__memcmp(const void* a, const void* b, ce_size n)
{
    ce_s32 ret;
    const ce_u8* aptr;
    const ce_u8* bptr;
    ce_size i;
    ce_u8 av;
    ce_u8 bv;

    ret  = 0;
    aptr = (const ce_u8*)a;
    bptr = (const ce_u8*)b;
    i    = 0;
    av   = (ce_u8)0u;
    bv   = (ce_u8)0u;

    if ((a == (const void*)0) || (b == (const void*)0)) {
        if (n != (ce_size)0) {
            ret = 0; /* Defensive neutral value; alternatively: define an error policy. */
        }
    } else {
        while ((i < n) && (ret == 0)) {
            av = aptr[i];
            bv = bptr[i];
            if (av != bv) {
                ret = (ce_s32)((ce_s32)av - (ce_s32)bv);
            }
            i++;
        }
    }

    return ret;
}

/**
 * @brief Computes the length of a C-string (excluding the terminating NUL).
 * @param s Input C-string pointer.
 * @return Length of the string, or 0 if s is NULL.
 * @note Single return policy.  
*/
static inline ce_size ce__strlen(const ce_char* s)
{
    ce_size len;
    ce_size i;

    len = (ce_size)0;
    i   = (ce_size)0;

    if (s != (const ce_char*)0) {
        while (s[i] != (ce_char)'\0') {
            i++;
        }
        len = i;
    }

    return len;
}

/**
 * @brief Appends up to n chars from src to dst, ensuring NUL-termination within dst_cap.
 * @param dst Destination C-string pointer.
 * @param dst_cap Total capacity of dst buffer in bytes (including space for terminating NUL).
 * @param src Source C-string pointer.
 * @param n Maximum number of characters to append from src.   
* @return Pointer to dst on success, CE_NULL (NULL) on failure.
* @note Defensive: returns CE_NULL if dst or src is NULL, or if dst_cap is 0.
* @note Always NUL-terminates if dst_cap > 0.
* @note Signature is Chaos-safe (adds dst_cap) to avoid overflow (differs from standard strncat).
* @note Single return policy.
 */
static inline ce_char* ce__strncat(ce_char* dst, ce_size dst_cap, const ce_char* src, ce_size n)
{
    ce_char* ret;
    ce_size dlen;
    ce_size i;
    ce_size j;
    ce_size room;

    ret  = dst;
    dlen = (ce_size)0;
    i    = (ce_size)0;
    j    = (ce_size)0;
    room = (ce_size)0;

    /* Defensive checks */
    if ((dst == (ce_char*)0) || (src == (const ce_char*)0) || (dst_cap == (ce_size)0)) {
        ret = (ce_char*)0;
    } else {
        /* Find current length of dst (bounded by dst_cap to be safe). */
        while ((dlen < dst_cap) && (dst[dlen] != (ce_char)'\0')) {
            dlen++;
        }

        /* If no NUL found within dst_cap, we cannot safely append. */
        if (dlen >= dst_cap) {
            ret = (ce_char*)0;
        } else {
            /* Compute remaining room excluding the terminating NUL we must keep. */
            room = dst_cap - dlen - (ce_size)1;

            /* Append up to n chars, but not exceeding room. */
            while ((i < n) && (i < room) && (src[i] != (ce_char)'\0')) {
                dst[dlen + i] = src[i];
                i++;
            }

            /* Always NUL-terminate (dst_cap > 0 guaranteed). */
            j = dlen + i;
            if (j < dst_cap) {
                dst[j] = (ce_char)'\0';
            } else {
                /* Should not happen due to room calc; defensive clamp. */
                dst[dst_cap - 1] = (ce_char)'\0';
            }
        }
    }

    return ret;
}

/**
 * @brief Copies n bytes from src to dst, supporting overlapping memory regions.
 * @param dst Destination pointer.
 * @param src Source pointer.
 * @param n Number of bytes to copy.
 * @return Pointer to dst on success, CE_NULL (NULL) on failure.
 * @note Defensive:
 *       * Returns CE_NULL if dst or src is NULL and n > 0.
 *       * Returns dst otherwise (even if n == 0).
 * @note Single return policy.  
*/
static inline void* ce__memmove(void* dst, const void* src, ce_size n)
{
    void* ret;
    ce_u8* dptr;
    const ce_u8* sptr;
    ce_size i;

    ret  = dst;
    dptr = (ce_u8*)dst;
    sptr = (const ce_u8*)src;
    i    = (ce_size)0;

    if ((dst == (void*)0) || (src == (const void*)0)) {
        if (n != (ce_size)0) {
            ret = (void*)0;
        }
    } else if (dptr == sptr) {
        /* Same pointer: nothing to do */
    } else if (dptr < sptr) {
        /* Forward copy (safe when dst before src) */
        while (i < n) {
            dptr[i] = sptr[i];
            i++;
        }
    } else {
        /* Backward copy (safe when dst overlaps src later in memory) */
        i = n;
        while (i > (ce_size)0) {
            i--;
            dptr[i] = sptr[i];
        }
    }

    return ret;
}


#endif /* CHAOS_STRING_H */
