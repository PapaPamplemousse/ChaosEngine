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
 * @file chaos_assets.c
 * @brief Implementation skeleton.
 */
#include "resources/chaos_assets.h"
#include "core/chaos_error.h"
#include "utility/chaos_math.h"
#include <SDL2/SDL.h>

/* ************* STATICS PROTOTYPES ************* */

static ce_u32 rd_u32(SDL_RWops* rw);
static ce_f32 rd_f32(SDL_RWops* rw);

/* ************* FUNCTIONS ************* */

ce_result ce_assets_load_stl_binary(ce_arena* arena, const ce_char* path, ce_mesh_t* out_mesh)
{
    ce_result  res = CE_OK;
    SDL_RWops* rw  = 0;
    ce_u8      hdr[80];
    ce_size    n     = 0U;
    ce_u32     tri   = 0U;
    ce_f32*    verts = 0;
    ce_u32*    inds  = 0;
    ce_u32     vi    = 0U;
    ce_u32     ii    = 0U;
    ce_f32     big   = 1.0e30f;
    ce_f32     dx, dy, dz;
    ce_f32     v0x, v1x, v2x;
    ce_f32     v0y, v1y, v2y;
    ce_f32     v0z, v1z, v2z;
    ce_u8      attr[2];
    if ((arena == 0) || (path == 0) || (out_mesh == 0))
    {
        CE_RAISE_ERROR(CHAOS_ERR_INVALID_ARGUMENT, "stl args");
        res = -CHAOS_ERR_INVALID_ARGUMENT;
    }
    else
    {
        rw = SDL_RWFromFile(path, "rb");
        if (rw == 0)
        {
            CE_RAISE_ERROR(CHAOS_ERR_RESOURCE_NOT_FOUND, "stl open");
            res = -CHAOS_ERR_RESOURCE_NOT_FOUND;
        }
        else
        {
            n = (ce_size)SDL_RWread(rw, hdr, 1, 80);
            if (n != 80U)
            {
                CE_RAISE_ERROR(CHAOS_ERR_RESOURCE_PARSE_FAILED, "stl hdr");
                res = -CHAOS_ERR_RESOURCE_PARSE_FAILED;
            }
            else
            {
                tri = rd_u32(rw);
                if (tri == 0U)
                {
                    CE_RAISE_ERROR(CHAOS_ERR_RESOURCE_PARSE_FAILED, "stl zero tris");
                    res = -CHAOS_ERR_RESOURCE_PARSE_FAILED;
                }
                else
                {
                    verts = (ce_f32*)ce_arena_alloc(arena, (ce_size)(tri * 3U * 3U * sizeof(ce_f32)), 16U);
                    inds  = (ce_u32*)ce_arena_alloc(arena, (ce_size)(tri * 3U * sizeof(ce_u32)), 16U);
                    /* init bounds */
                    out_mesh->bounds.min_v.x = big;
                    out_mesh->bounds.min_v.y = big;
                    out_mesh->bounds.min_v.z = big;
                    out_mesh->bounds.max_v.x = -big;
                    out_mesh->bounds.max_v.y = -big;
                    out_mesh->bounds.max_v.z = -big;

                    if ((verts == 0) || (inds == 0))
                    {
                        CE_RAISE_ERROR(CHAOS_ERR_OUT_OF_MEMORY, "stl arena oom");
                        res = -CHAOS_ERR_OUT_OF_MEMORY;
                    }
                    else
                    {
                        for (ce_u32 t = 0U; t < tri; ++t)
                        {
                            (void)rd_f32(rw);
                            (void)rd_f32(rw);
                            (void)rd_f32(rw);
                            v0x = rd_f32(rw), v0y = rd_f32(rw), v0z = rd_f32(rw);
                            v1x = rd_f32(rw), v1y = rd_f32(rw), v1z = rd_f32(rw);
                            v2x = rd_f32(rw), v2y = rd_f32(rw), v2z = rd_f32(rw);

                            (void)SDL_RWread(rw, attr, 1, 2);
                            verts[vi + 0] = v0x;
                            verts[vi + 1] = v0y;
                            verts[vi + 2] = v0z;
                            verts[vi + 3] = v1x;
                            verts[vi + 4] = v1y;
                            verts[vi + 5] = v1z;
                            verts[vi + 6] = v2x;
                            verts[vi + 7] = v2y;
                            verts[vi + 8] = v2z;
                            inds[ii + 0]  = (vi / 3U) + 0U;
                            inds[ii + 1]  = (vi / 3U) + 1U;
                            inds[ii + 2]  = (vi / 3U) + 2U;
                            vi += 9U;
                            ii += 3U;
#define CE_UPD_BOUNDS(px, py, pz)                                                                                                                                                  \
    do                                                                                                                                                                             \
    {                                                                                                                                                                              \
        if ((px) < out_mesh->bounds.min_v.x)                                                                                                                                       \
        {                                                                                                                                                                          \
            out_mesh->bounds.min_v.x = (px);                                                                                                                                       \
        }                                                                                                                                                                          \
        if ((py) < out_mesh->bounds.min_v.y)                                                                                                                                       \
        {                                                                                                                                                                          \
            out_mesh->bounds.min_v.y = (py);                                                                                                                                       \
        }                                                                                                                                                                          \
        if ((pz) < out_mesh->bounds.min_v.z)                                                                                                                                       \
        {                                                                                                                                                                          \
            out_mesh->bounds.min_v.z = (pz);                                                                                                                                       \
        }                                                                                                                                                                          \
        if ((px) > out_mesh->bounds.max_v.x)                                                                                                                                       \
        {                                                                                                                                                                          \
            out_mesh->bounds.max_v.x = (px);                                                                                                                                       \
        }                                                                                                                                                                          \
        if ((py) > out_mesh->bounds.max_v.y)                                                                                                                                       \
        {                                                                                                                                                                          \
            out_mesh->bounds.max_v.y = (py);                                                                                                                                       \
        }                                                                                                                                                                          \
        if ((pz) > out_mesh->bounds.max_v.z)                                                                                                                                       \
        {                                                                                                                                                                          \
            out_mesh->bounds.max_v.z = (pz);                                                                                                                                       \
        }                                                                                                                                                                          \
    } while (0)

                            CE_UPD_BOUNDS(v0x, v0y, v0z);
                            CE_UPD_BOUNDS(v1x, v1y, v1z);
                            CE_UPD_BOUNDS(v2x, v2y, v2z);
#undef CE_UPD_BOUNDS
                        }
                        out_mesh->vertices      = verts;
                        out_mesh->vertex_count  = tri * 3U;
                        out_mesh->indices       = inds;
                        out_mesh->index_count   = tri * 3U;
                        dx                      = out_mesh->bounds.max_v.x - out_mesh->bounds.center.x;
                        dy                      = out_mesh->bounds.max_v.y - out_mesh->bounds.center.y;
                        dz                      = out_mesh->bounds.max_v.z - out_mesh->bounds.center.z;
                        out_mesh->bounds.radius = ce_sqrt((dx * dx) + (dy * dy) + (dz * dz));
                        res                     = CE_OK;
                    }
                }
            }
            SDL_RWclose(rw);
        }
    }
    return res;
}

static ce_u32 rd_u32(SDL_RWops* rw)
{
    ce_u8   b[4] = {0, 0, 0, 0};
    ce_size n    = (ce_size)SDL_RWread(rw, b, 1, 4);
    if (n != 4U)
    {
        return 0U;
    }
    return (ce_u32)b[0] | ((ce_u32)b[1] << 8) | ((ce_u32)b[2] << 16) | ((ce_u32)b[3] << 24);
}

static ce_f32 rd_f32(SDL_RWops* rw)
{
    ce_u8   b[4] = {0, 0, 0, 0};
    ce_size n    = (ce_size)SDL_RWread(rw, b, 1, 4);
    if (n != 4U)
    {
        return 0.0f;
    }
    union
    {
        ce_u32 u;
        ce_f32 f;
    } cvt;
    cvt.u = (ce_u32)b[0] | ((ce_u32)b[1] << 8) | ((ce_u32)b[2] << 16) | ((ce_u32)b[3] << 24);
    return cvt.f;
}
