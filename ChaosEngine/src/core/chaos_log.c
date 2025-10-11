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
 * @file chaos_log.c
 * @brief Minimal logging implementation for ChaosEngine.
 */

#include "core/chaos_log.h"

#include "core/chaos_error.h"
#include "utility/chaos_string.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/* ************************************************************************** */
/* INTERNAL STATE                                                             */
/* ************************************************************************** */

/** @brief Currently selected minimum log level. */
static ce_log_level g_log_level = CE_LOG_INFO;
/** @brief Enable printing timestamps when CE_TRUE. */
static ce_bool g_log_timestamp_enabled = CE_TRUE;

/* ************************************************************************** */
/* INTERNAL HELPERS                                                           */
/* ************************************************************************** */

/**
 * @brief Returns the printable label associated with a log level.
 */
static const ce_char* ce__log_level_label(ce_log_level level)
{
    switch (level)
    {
        case CE_LOG_TRACE: return "TRACE";
        case CE_LOG_DEBUG: return "DEBUG";
        case CE_LOG_INFO:  return "INFO";
        case CE_LOG_WARN:  return "WARN";
        case CE_LOG_ERROR: return "ERROR";
        case CE_LOG_FATAL: return "FATAL";
        default:           return "LOG";
    }
}

/**
 * @brief Returns the FILE* stream to use for a given level.
 */
static FILE* ce__log_stream_for_level(ce_log_level level)
{
    if (level >= CE_LOG_ERROR)
    {
        return stderr;
    }
    return stdout;
}

/* ************************************************************************** */
/* PUBLIC API                                                                 */
/* ************************************************************************** */

void ce_log_set_level(ce_log_level level)
{
    g_log_level = level;
}

ce_log_level ce_log_get_level(void)
{
    return g_log_level;
}

void ce_log_enable_timestamp(ce_bool enabled)
{
    g_log_timestamp_enabled = enabled;
}

void ce_log_message(ce_log_level level, const ce_char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ce_log_messagev(level, fmt, args);
    va_end(args);
}

void ce_log_messagev(ce_log_level level, const ce_char* fmt, va_list args)
{
    FILE*       stream;
    ce_char     buffer[1024];
    ce_size     len;
    time_t      now;
    struct tm   local_tm;
    ce_char     timestamp[64];

    if (fmt == (const ce_char*)0)
    {
        return;
    }

    if (level < g_log_level)
    {
        return;
    }

    stream = ce__log_stream_for_level(level);
    len    = (ce_size)0;

    if (g_log_timestamp_enabled == CE_TRUE)
    {
        now = time((time_t*)0);
#if defined(_MSC_VER)
        localtime_s(&local_tm, &now);
#else
        localtime_r(&now, &local_tm);
#endif
        (void)strftime(timestamp, sizeof(timestamp), "%H:%M:%S", &local_tm);
        fprintf(stream, "[%s] %s | ", ce__log_level_label(level), timestamp);
    }
    else
    {
        fprintf(stream, "[%s] ", ce__log_level_label(level));
    }

#if defined(_MSC_VER)
    len = (ce_size)_vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);
#else
    len = (ce_size)vsnprintf(buffer, sizeof(buffer), fmt, args);
#endif

    if (len >= (ce_size)sizeof(buffer))
    {
        buffer[sizeof(buffer) - 1U] = '\0';
    }

    fputs(buffer, stream);
    fputc('\n', stream);
    fflush(stream);

    if (level >= CE_LOG_ERROR)
    {
        CE_RAISE_ERROR(CHAOS_ERR_UNKNOWN, buffer);
    }
}
