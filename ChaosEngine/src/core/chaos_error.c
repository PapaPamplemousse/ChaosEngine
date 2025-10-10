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
 * @file chaos_error.c
 * @brief Implementation of ChaosEngine error system.
 */

#include "core/chaos_error.h"
#include "utility/chaos_string.h"  /* For ce__strlen / ce__memcpy */

/* =========================================================================================
   INTERNAL GLOBAL CONTEXT
   ========================================================================================= */

static ce_error_context g_ce_error_ctx;

/* =========================================================================================
   INTERNAL HELPERS 
   ========================================================================================= */
static ce_size internal__clamp_msg_copy(ce_char* dst, ce_size dst_cap, const ce_char* src)
{
    ce_size copied;
    ce_size i;
    ce_size limit;

    copied = (ce_size)0;
    i      = (ce_size)0;
    limit  = (dst_cap > (ce_size)0) ? (dst_cap - (ce_size)1) : (ce_size)0;

    if ((dst == (ce_char*)0) || (dst_cap == (ce_size)0)) {
        /* Nothing to copy */
    } else if (src == (const ce_char*)0) {
        dst[0] = (ce_char)'\0';
    } else {
        while ((i < limit) && (src[i] != (ce_char)'\0')) {
            dst[i] = src[i];
            i++;
        }
        dst[i] = (ce_char)'\0';
        copied = i;
    }

    return copied;
}

/* =========================================================================================
   PUBLIC API
   ========================================================================================= */

void ce_error_init(ce_error_context* ctx)
{
    ce_size i;

    i = (ce_size)0;

    if (ctx != (ce_error_context*)0) {
        ctx->head = (ce_size)0;
        ctx->count = (ce_size)0;
        ctx->min_severity = (ce_u16)CE_SEV_INFO;
        ctx->handler = (ce_error_handler)0;
        ctx->handler_user = (void*)0;

        while (i < CE_ERROR_RING_CAP) {
            ctx->ring[i].category = (ce_u16)0u;
            ctx->ring[i].severity = (ce_u16)0u;
            ctx->ring[i].code = (ce_u16)0u;
            ctx->ring[i].timestamp_ms = (ce_u32)0u;
            ctx->ring[i].line = (ce_u32)0u;
            ctx->ring[i].file = (const ce_char*)0;
            ctx->ring[i].message[0] = (ce_char)'\0';
            i++;
        }
    }
}

void ce_error_set_min_severity(ce_error_context* ctx, ce_u16 min_sev)
{
    if (ctx != (ce_error_context*)0) {
        ctx->min_severity = min_sev;
    }
}

void ce_error_set_handler(ce_error_context* ctx, ce_error_handler handler, void* user)
{
    if (ctx != (ce_error_context*)0) {
        ctx->handler = handler;
        ctx->handler_user = user;
    }
}

ce_result ce_error_emit_ex(ce_error_context* ctx,
                           ce_u16 category,
                           ce_u16 severity,
                           ce_u16 code,
                           const ce_char* file,
                           ce_u32 line,
                           ce_u32 timestamp_ms,
                           const ce_char* message)
{
    ce_result ret;
    ce_size idx;
    ce_error_event* evt;

    ret = (ce_result)0;
    idx = (ce_size)0;
    evt = (ce_error_event*)0;

    if (ctx == (ce_error_context*)0) {
        ret = (ce_result)(-((ce_s32)CHAOS_ERR_INVALID_ARGUMENT));
    } else if (severity < ctx->min_severity) {
        /* Filtered out: do nothing, but considered success */
        ret = (ce_result)0;
    } else {
        idx = ctx->head;
        evt = &ctx->ring[idx];

        evt->category = category;
        evt->severity = severity;
        evt->code = code;
        evt->timestamp_ms = timestamp_ms;
        evt->line = line;
        evt->file = file;

        internal__clamp_msg_copy(evt->message, (ce_size)CE_ERROR_MSG_MAX, message);

        /* Advance ring pointers */
        ctx->head = (idx + (ce_size)1) % CE_ERROR_RING_CAP;
        if (ctx->count < CE_ERROR_RING_CAP) {
            ctx->count++;
        }

        /* Invoke handler if any */
        if (ctx->handler != (ce_error_handler)0) {
            ctx->handler(evt, ctx->handler_user);
        }

        ret = (ce_result)0;
    }

    return ret;
}

const ce_error_event* ce_error_last(const ce_error_context* ctx)
{
    const ce_error_event* out;
    ce_size pos;

    out = (const ce_error_event*)0;
    pos = (ce_size)0;

    if ((ctx != (const ce_error_context*)0) && (ctx->count > (ce_size)0)) {
        if (ctx->head == (ce_size)0) {
            pos = CE_ERROR_RING_CAP - (ce_size)1;
        } else {
            pos = ctx->head - (ce_size)1;
        }
        out = &ctx->ring[pos];
    }

    return out;
}

ce_result ce_error_pop(ce_error_context* ctx, ce_error_event* out)
{
    ce_result ret;
    ce_size start_index;
    ce_size oldest_index;

    ret = (ce_result)0;
    start_index = (ce_size)0;
    oldest_index = (ce_size)0;

    if ((ctx == (ce_error_context*)0) || (out == (ce_error_event*)0)) {
        ret = (ce_result)(-((ce_s32)CHAOS_ERR_INVALID_ARGUMENT));
    } else if (ctx->count == (ce_size)0) {
        ret = (ce_result)(-((ce_s32)CHAOS_ERR_UNKNOWN));
    } else {
        /* Oldest = (head - count + CAP) % CAP */
        start_index = (ctx->head + CE_ERROR_RING_CAP - ctx->count) % CE_ERROR_RING_CAP;
        oldest_index = start_index;

        /* Copy out */
        *out = ctx->ring[oldest_index];

        /* Consume one item by decreasing count */
        ctx->count--;
        ret = (ce_result)0;
    }

    return ret;
}

ce_size ce_error_count(const ce_error_context* ctx)
{
    ce_size c;

    c = (ce_size)0;
    if (ctx != (const ce_error_context*)0) {
        c = ctx->count;
    }
    return c;
}

const ce_char* ce_error_severity_str(ce_u16 sev)
{
    const ce_char* s;

    s = (const ce_char*)"UNKNOWN";
    if (sev == (ce_u16)CE_SEV_INFO) {
        s = (const ce_char*)"INFO";
    } else if (sev == (ce_u16)CE_SEV_WARN) {
        s = (const ce_char*)"WARN";
    } else if (sev == (ce_u16)CE_SEV_ERROR) {
        s = (const ce_char*)"ERROR";
    } else if (sev == (ce_u16)CE_SEV_CRITICAL) {
        s = (const ce_char*)"CRITICAL";
    } else if (sev == (ce_u16)CE_SEV_FATAL) {
        s = (const ce_char*)"FATAL";
    }

    return s;
}

const ce_char* ce_error_category_str(ce_u16 cat)
{
    const ce_char* s;

    s = (const ce_char*)"UNKNOWN";
    if (cat == (ce_u16)CE_CAT_CORE) {
        s = (const ce_char*)"CORE";
    } else if (cat == (ce_u16)CE_CAT_PLATFORM) {
        s = (const ce_char*)"PLATFORM";
    } else if (cat == (ce_u16)CE_CAT_GFX) {
        s = (const ce_char*)"GFX";
    } else if (cat == (ce_u16)CE_CAT_AUDIO) {
        s = (const ce_char*)"AUDIO";
    } else if (cat == (ce_u16)CE_CAT_PHYSICS) {
        s = (const ce_char*)"PHYSICS";
    } else if (cat == (ce_u16)CE_CAT_RESOURCES) {
        s = (const ce_char*)"RESOURCES";
    } else if (cat == (ce_u16)CE_CAT_RUNTIME) {
        s = (const ce_char*)"RUNTIME";
    } else if (cat == (ce_u16)CE_CAT_DEBUG) {
        s = (const ce_char*)"DEBUG";
    } else if (cat == (ce_u16)CE_CAT_SYSTEM) {
        s = (const ce_char*)"SYSTEM";
    }

    return s;
}

/* =========================================================================================
   GLOBAL SINGLETON
   ========================================================================================= */

ce_error_context* ce_error_global(void)
{
    /* Lazy-initialize on first call (idempotent). */
    static ce_bool initialized = CE_FALSE;

    if (initialized == CE_FALSE) {
        ce_error_init(&g_ce_error_ctx);
        initialized = CE_TRUE;
    }

    return &g_ce_error_ctx;
}

