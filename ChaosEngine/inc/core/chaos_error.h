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
 * @file chaos_error.h
 * @brief ChaosEngine error system: categories, severities, codes, events, and API.
 *
 * Provides a portable error/event system with:
 *  - Categories (CORE, PLATFORM, GFX, AUDIO, PHYSICS, RESOURCES, RUNTIME, DEBUG, SYSTEM)
 *  - Severities (INFO, WARN, ERROR, CRITICAL, FATAL)
 *  - Numeric error codes per your specification
 *  - An internal ring buffer of recent events
 *  - A user-defined event handler callback
 *  - Convenience macros to raise errors with file/line
 *
 *
 * @author PapaPamplemousse
 * @version 1.0.0
 */

#ifndef CHAOS_ERROR_H
#define CHAOS_ERROR_H

#include "core/chaos_types.h"

/* =========================================================================================
   CONFIGURATION
   ========================================================================================= */

/**
 * @brief Maximum number of stored events in the ring buffer.
 */
#ifndef CE_ERROR_RING_CAP
#define CE_ERROR_RING_CAP ((ce_size)128)
#endif

/**
 * @brief Maximum length (in bytes) of an error message, including the NUL terminator.
 */
#ifndef CE_ERROR_MSG_MAX
#define CE_ERROR_MSG_MAX ((ce_size)128)
#endif

/* =========================================================================================
   SEVERITY, CATEGORY, CODE
   ========================================================================================= */

/**
 * @brief Severity levels for error/events.
 */
typedef enum ce_severity_e
{
    CE_SEV_INFO     = 0,
    CE_SEV_WARN     = 1,
    CE_SEV_ERROR    = 2,
    CE_SEV_CRITICAL = 3,
    CE_SEV_FATAL    = 4
} ce_severity;

/**
 * @brief Error categories (subsystems).
 */
typedef enum ce_category_e
{
    CE_CAT_CORE      = 1,
    CE_CAT_PLATFORM  = 2,
    CE_CAT_GFX       = 3,
    CE_CAT_AUDIO     = 4,
    CE_CAT_PHYSICS   = 5,
    CE_CAT_RESOURCES = 6,
    CE_CAT_RUNTIME   = 7,
    CE_CAT_DEBUG     = 8,
    CE_CAT_SYSTEM    = 9
} ce_category;

/**
 * @brief Error/result code type. Zero means success.
 *
 * Non-zero indicates failure; by convention we return negative values in functions
 * for errors, where the absolute value corresponds to a CHAOS_ERR_* code below.
 */
typedef ce_s32 ce_result;

/** @brief Success and failure helpers. */
#define CE_OK ((ce_result)0)
#define CE_FAILED(r) ((r) != (ce_result)0)
#define CE_SUCCEEDED(r) ((r) == (ce_result)0)

/* ---------- Core (100–1..) ---------- */
#define CHAOS_ERR_UNKNOWN ((ce_u16)100)
#define CHAOS_ERR_UNIMPLEMENTED ((ce_u16)101)
#define CHAOS_ERR_INVALID_ARGUMENT ((ce_u16)102)
#define CHAOS_ERR_NULL_POINTER ((ce_u16)103)
#define CHAOS_ERR_OVERFLOW ((ce_u16)104)
#define CHAOS_ERR_UNDERFLOW ((ce_u16)105)
#define CHAOS_ERR_ALLOCATION_FAILED ((ce_u16)106)
#define CHAOS_ERR_OUT_OF_MEMORY ((ce_u16)107)
#define CHAOS_ERR_INDEX_OUT_OF_BOUNDS ((ce_u16)108)
#define CHAOS_ERR_ALIGNMENT ((ce_u16)109)
#define CHAOS_ERR_STATE_CORRUPTED ((ce_u16)110)

/* ---------- Platform (200–2..) ---------- */
#define CHAOS_ERR_PLATFORM_UNSUPPORTED ((ce_u16)200)
#define CHAOS_ERR_PLATFORM_INIT_FAILED ((ce_u16)201)
#define CHAOS_ERR_WINDOW_CREATION_FAILED ((ce_u16)202)
#define CHAOS_ERR_WINDOW_DESTROY_FAILED ((ce_u16)203)
#define CHAOS_ERR_INPUT_DEVICE_FAILURE ((ce_u16)204)
#define CHAOS_ERR_THREAD_CREATE_FAILED ((ce_u16)205)
#define CHAOS_ERR_MUTEX_FAILURE ((ce_u16)206)
#define CHAOS_ERR_TIMER_FAILURE ((ce_u16)207)
#define CHAOS_ERR_EVENT_POLL_FAILED ((ce_u16)208)
#define CHAOS_ERR_FILE_NOT_FOUND ((ce_u16)209)
#define CHAOS_ERR_FILE_ACCESS_DENIED ((ce_u16)210)
#define CHAOS_ERR_IO_FAILURE ((ce_u16)211)
#define CHAOS_ERR_PATH_TOO_LONG ((ce_u16)212)
#define CHAOS_ERR_ENV_NOT_AVAILABLE ((ce_u16)213)

/* ---------- GFX (300–3..) ---------- */
#define CHAOS_ERR_GFX_INIT_FAILED ((ce_u16)300)
#define CHAOS_ERR_GFX_BACKEND_MISSING ((ce_u16)301)
#define CHAOS_ERR_GFX_DEVICE_LOST ((ce_u16)302)
#define CHAOS_ERR_GFX_SHADER_COMPILE_FAILED ((ce_u16)303)
#define CHAOS_ERR_GFX_SHADER_LINK_FAILED ((ce_u16)304)
#define CHAOS_ERR_GFX_INVALID_TEXTURE ((ce_u16)305)
#define CHAOS_ERR_GFX_UPLOAD_FAILED ((ce_u16)306)
#define CHAOS_ERR_GFX_OUT_OF_VRAM ((ce_u16)307)
#define CHAOS_ERR_GFX_COMMAND_OVERFLOW ((ce_u16)308)
#define CHAOS_ERR_GFX_CONTEXT_UNBOUND ((ce_u16)309)
#define CHAOS_ERR_GFX_UNSUPPORTED_FEATURE ((ce_u16)310)

/* ---------- Audio (400–4..) ---------- */
#define CHAOS_ERR_AUDIO_INIT_FAILED ((ce_u16)400)
#define CHAOS_ERR_AUDIO_DEVICE_LOST ((ce_u16)401)
#define CHAOS_ERR_AUDIO_UNSUPPORTED_FORMAT ((ce_u16)402)
#define CHAOS_ERR_AUDIO_DECODE_FAILED ((ce_u16)403)
#define CHAOS_ERR_AUDIO_BUFFER_OVERFLOW ((ce_u16)404)
#define CHAOS_ERR_AUDIO_CHANNEL_BUSY ((ce_u16)405)
#define CHAOS_ERR_AUDIO_STREAM_FAILURE ((ce_u16)406)
#define CHAOS_ERR_AUDIO_FILE_NOT_FOUND ((ce_u16)407)
#define CHAOS_ERR_AUDIO_VOLUME_INVALID ((ce_u16)408)
#define CHAOS_ERR_AUDIO_MIXER_FAILURE ((ce_u16)409)

/* ---------- Physics (500–5..) ---------- */
#define CHAOS_ERR_PHYSICS_WORLD_NOT_INITIALIZED ((ce_u16)500)
#define CHAOS_ERR_PHYSICS_INTEGRATION_FAILED ((ce_u16)501)
#define CHAOS_ERR_PHYSICS_NAN_DETECTED ((ce_u16)502)
#define CHAOS_ERR_PHYSICS_BODY_INVALID ((ce_u16)503)
#define CHAOS_ERR_PHYSICS_COLLISION_MISSED ((ce_u16)504)
#define CHAOS_ERR_PHYSICS_BROADPHASE_FAILED ((ce_u16)505)
#define CHAOS_ERR_PHYSICS_CONSTRAINT_BROKEN ((ce_u16)506)

/* ---------- Resources (600–6..) ---------- */
#define CHAOS_ERR_RESOURCE_NOT_FOUND ((ce_u16)600)
#define CHAOS_ERR_RESOURCE_LOAD_FAILED ((ce_u16)601)
#define CHAOS_ERR_RESOURCE_PARSE_FAILED ((ce_u16)602)
#define CHAOS_ERR_RESOURCE_CORRUPTED ((ce_u16)603)
#define CHAOS_ERR_RESOURCE_TYPE_MISMATCH ((ce_u16)604)
#define CHAOS_ERR_RESOURCE_OUT_OF_CACHE ((ce_u16)605)
#define CHAOS_ERR_RESOURCE_HANDLE_INVALID ((ce_u16)606)
#define CHAOS_ERR_RESOURCE_DEPENDENCY_MISSING ((ce_u16)607)

/* ---------- Runtime (700–7..) ---------- */
#define CHAOS_ERR_ENGINE_NOT_INITIALIZED ((ce_u16)700)
#define CHAOS_ERR_ENGINE_ALREADY_RUNNING ((ce_u16)701)
#define CHAOS_ERR_ENGINE_LOOP_FAILED ((ce_u16)702)
#define CHAOS_ERR_ENGINE_FRAME_OVERRUN ((ce_u16)703)
#define CHAOS_ERR_JOB_SYSTEM_FAILED ((ce_u16)704)
#define CHAOS_ERR_JOB_QUEUE_OVERFLOW ((ce_u16)705)
#define CHAOS_ERR_PROFILER_DISABLED ((ce_u16)706)
#define CHAOS_ERR_EVENT_DISPATCH_FAILED ((ce_u16)707)

/* ---------- Utility (800–8..) ---------- */
#define CHAOS_ERR_MATH_DIV_BY_ZERO ((ce_u16)800)
#define CHAOS_ERR_SQRT_OF_NEGATIVE ((ce_u16)801)

/* ---------- Debug/Log (900–9..) ---------- */
#define CHAOS_WARN_DEPRECATED_FUNCTION ((ce_u16)900)
#define CHAOS_WARN_UNOPTIMIZED_PATH ((ce_u16)901)
#define CHAOS_WARN_UNINITIALIZED_VALUE ((ce_u16)902)
#define CHAOS_WARN_MEMORY_LEAK ((ce_u16)903)
#define CHAOS_WARN_PERFORMANCE_DROP ((ce_u16)904)
#define CHAOS_WARN_PRECISION_LOSS ((ce_u16)905)
#define CHAOS_EVENT_INFO ((ce_u16)906)
#define CHAOS_EVENT_DEBUG ((ce_u16)907)
#define CHAOS_EVENT_FRAME_START ((ce_u16)908)
#define CHAOS_EVENT_FRAME_END ((ce_u16)909)
#define CHAOS_EVENT_INPUT_RECEIVED ((ce_u16)910)
#define CHAOS_EVENT_ASSET_RELOADED ((ce_u16)911)

/* ---------- System/Critical (1000–10..) ---------- */
#define CHAOS_ERR_FATAL_SIGNAL ((ce_u16)1000)
#define CHAOS_ERR_SHUTDOWN_REQUESTED ((ce_u16)1001)
#define CHAOS_ERR_TIMEOUT ((ce_u16)1002)
#define CHAOS_ERR_CONFIG_INVALID ((ce_u16)1003)
#define CHAOS_ERR_DEPENDENCY_MISSING ((ce_u16)1004)
#define CHAOS_ERR_INTERNAL_ASSERT ((ce_u16)1005)
#define CHAOS_ERR_PANIC ((ce_u16)1006)

/* =========================================================================================
   EVENT & CONTEXT
   ========================================================================================= */

/**
 * @brief Single error/event record stored by the system.
 *
 * @note The message is stored by value (fixed-size) to avoid heap allocations.
 */
typedef struct ce_error_event_s
{
    ce_u16         category;                  /**< One of ce_category. */
    ce_u16         severity;                  /**< One of ce_severity. */
    ce_u16         code;                      /**< CHAOS_ERR_* or CHAOS_WARN_* or CHAOS_EVENT_*. */
    ce_u32         timestamp_ms;              /**< Optional timestamp (set by caller or platform layer). */
    ce_u32         line;                      /**< Source line where raised. */
    const ce_char* file;                      /**< Source file pointer (const, not owned). */
    ce_char        message[CE_ERROR_MSG_MAX]; /**< Message copied and NUL-terminated. */
} ce_error_event;

/**
 * @brief Error handler callback signature.
 *
 * Called after an event is enqueued successfully.
 */
typedef void (*ce_error_handler)(const ce_error_event* evt, void* user_data);

/**
 * @brief Error system context with ring buffer and callback hook.
 */
typedef struct ce_error_context_s
{
    ce_error_event   ring[CE_ERROR_RING_CAP]; /**< Circular event buffer. */
    ce_size          head;                    /**< Next write index. */
    ce_size          count;                   /**< Number of valid items (<= CAP). */
    ce_u16           min_severity;            /**< Filter: events below this are ignored. */
    ce_error_handler handler;                 /**< Optional event sink. */
    void*            handler_user;            /**< User pointer for handler. */
} ce_error_context;

/* =========================================================================================
   PUBLIC API
   ========================================================================================= */

/**
 * @brief Initialize an error context.
 * @param ctx Context pointer (must not be NULL).
 */
void ce_error_init(ce_error_context* ctx);

/**
 * @brief Set minimal accepted severity. Events below are dropped.
 * @param ctx Context pointer.
 * @param min_sev Minimal severity to accept (inclusive).
 */
void ce_error_set_min_severity(ce_error_context* ctx, ce_u16 min_sev);

/**
 * @brief Install a user callback to observe events.
 * @param ctx Context pointer.
 * @param handler Callback (nullable to clear).
 * @param user User pointer passed back to the callback.
 */
void ce_error_set_handler(ce_error_context* ctx, ce_error_handler handler, void* user);

/**
 * @brief Emit a new error/event into the context.
 *
 * The message is copied and NUL-terminated. If ctx is NULL, this is a no-op and
 * returns a negative result (invalid argument).
 *
 * @param ctx Context pointer.
 * @param category One of ce_category.
 * @param severity One of ce_severity.
 * @param code Code within the chosen category.
 * @param file Optional file string (can be __FILE__).
 * @param line Source line number (can be __LINE__).
 * @param timestamp_ms Optional timestamp in milliseconds (0 if unknown).
 * @param message Optional message string (can be NULL).
 * @return CE_OK on success, negative on error (e.g., -CHAOS_ERR_INVALID_ARGUMENT).
 */
ce_result ce_error_emit_ex(ce_error_context* ctx, ce_u16 category, ce_u16 severity, ce_u16 code, const ce_char* file, ce_u32 line, ce_u32 timestamp_ms, const ce_char* message);

/**
 * @brief Convenience wrapper with timestamp_ms = 0.
 */
static inline ce_result ce_error_emit(ce_error_context* ctx, ce_u16 category, ce_u16 severity, ce_u16 code, const ce_char* file, ce_u32 line, const ce_char* message)
{
    return ce_error_emit_ex(ctx, category, severity, code, file, line, (ce_u32)0u, message);
}

/**
 * @brief Retrieve the last event (most recently written).
 * @param ctx Context pointer.
 * @return Pointer to the last event, or NULL if none.
 */
const ce_error_event* ce_error_last(const ce_error_context* ctx);

/**
 * @brief Pop the oldest event into @p out.
 * @param ctx Context pointer.
 * @param out Output event (must not be NULL).
 * @return CE_OK on success, negative if empty or invalid argument.
 */
ce_result ce_error_pop(ce_error_context* ctx, ce_error_event* out);

/**
 * @brief Get number of stored events.
 * @param ctx Context pointer.
 * @return Count (0 if ctx is NULL).
 */
ce_size ce_error_count(const ce_error_context* ctx);

/**
 * @brief Translate a severity value to a static C-string.
 * @param sev Severity.
 * @return Static string (never NULL).
 */
const ce_char* ce_error_severity_str(ce_u16 sev);

/**
 * @brief Translate a category value to a static C-string.
 * @param cat Category.
 * @return Static string (never NULL).
 */
const ce_char* ce_error_category_str(ce_u16 cat);

/* =========================================================================================
   GLOBAL SINGLETON
   ========================================================================================= */

/**
 * @brief Access the global error context singleton.
 * @return Pointer to the internal global context.
 */
ce_error_context* ce_error_global(void);

/**
 * @brief Macros to raise events on the global context with file/line.
 */
#define CE_RAISE_INFO(code, msg) ce_error_emit(ce_error_global(), CE_CAT_DEBUG, CE_SEV_INFO, (code), __FILE__, (ce_u32)__LINE__, (msg))
#define CE_RAISE_WARN(code, msg) ce_error_emit(ce_error_global(), CE_CAT_DEBUG, CE_SEV_WARN, (code), __FILE__, (ce_u32)__LINE__, (msg))
#define CE_RAISE_ERROR(code, msg) ce_error_emit(ce_error_global(), CE_CAT_CORE, CE_SEV_ERROR, (code), __FILE__, (ce_u32)__LINE__, (msg))
#define CE_RAISE_CRITICAL(code, msg) ce_error_emit(ce_error_global(), CE_CAT_SYSTEM, CE_SEV_CRITICAL, (code), __FILE__, (ce_u32)__LINE__, (msg))
#define CE_RAISE_FATAL(code, msg) ce_error_emit(ce_error_global(), CE_CAT_SYSTEM, CE_SEV_FATAL, (code), __FILE__, (ce_u32)__LINE__, (msg))

#endif /* CHAOS_ERROR_H */
