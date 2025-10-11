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
 * @file chaos_memory_arena.c
 * @brief Implementation skeleton.
 */
#include "core/chaos_memory.h"
#include "core/chaos_error.h"

static ce_size ce_align_up(ce_size x, ce_size a)
{
    ce_size out = x;
    if (a != 0U)
    {
        ce_size mask = a - 1U;
        out          = (x + mask) & (~mask);
    }
    return out;
}
void ce_arena_init(ce_arena* a, void* memory, ce_size capacity)
{
    if ((a == 0) || (memory == 0) || (capacity == 0U))
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "ce_arena_init invalid");
    }
    else
    {
        a->base = (ce_u8*)memory;
        a->cap  = capacity;
        a->off  = 0U;
    }
}
void* ce_arena_alloc(ce_arena* a, ce_size size, ce_size align)
{
    void* ptr = 0;
    if ((a == 0) || (size == 0U))
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "ce_arena_alloc invalid");
    }
    else
    {
        if (align == 0U)
        {
            align = 8U;
        }
        ce_size aligned = ce_align_up(a->off, align);
        ce_size next    = aligned + size;
        if (next > a->cap)
        {
            CE_RAISE_ERROR(CHAOS_ERR_OUT_OF_MEMORY, "arena OOM");
        }
        else
        {
            ptr    = (void*)(a->base + aligned);
            a->off = next;
        }
    }
    return ptr;
}
void ce_arena_reset(ce_arena* a)
{
    if (a == 0)
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "ce_arena_reset invalid");
    }
    else
    {
        a->off = 0U;
    }
}
