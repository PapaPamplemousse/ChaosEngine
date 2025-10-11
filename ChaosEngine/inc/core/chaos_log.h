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
 * @file chaos_log.h
 * @brief Logging API.
 * @author PapaPamplemousse
 */
#ifndef CHAOS_LOG_H
#define CHAOS_LOG_H

#include "core/chaos_types.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Runtime log severity levels.
 */
typedef enum ce_log_level_e
{
    CE_LOG_TRACE = 0,
    CE_LOG_DEBUG = 1,
    CE_LOG_INFO  = 2,
    CE_LOG_WARN  = 3,
    CE_LOG_ERROR = 4,
    CE_LOG_FATAL = 5
} ce_log_level;

/**
 * @brief Sets the minimum level that will be printed to the log sink.
 */
void ce_log_set_level(ce_log_level level);

/**
 * @brief Returns the current logging level threshold.
 */
ce_log_level ce_log_get_level(void);

/**
 * @brief Enables or disables timestamps in log output.
 */
void ce_log_enable_timestamp(ce_bool enabled);

/**
 * @brief Writes a formatted log message.
 */
void ce_log_message(ce_log_level level, const ce_char* fmt, ...);

/**
 * @brief Variant of @ref ce_log_message that accepts a va_list.
 */
void ce_log_messagev(ce_log_level level, const ce_char* fmt, va_list args);

/* Convenience macros */
#define CE_LOG_TRACE(...) ce_log_message(CE_LOG_TRACE, __VA_ARGS__)
#define CE_LOG_DEBUG(...) ce_log_message(CE_LOG_DEBUG, __VA_ARGS__)
#define CE_LOG_INFO(...)  ce_log_message(CE_LOG_INFO,  __VA_ARGS__)
#define CE_LOG_WARN(...)  ce_log_message(CE_LOG_WARN,  __VA_ARGS__)
#define CE_LOG_ERROR(...) ce_log_message(CE_LOG_ERROR, __VA_ARGS__)
#define CE_LOG_FATAL(...) ce_log_message(CE_LOG_FATAL, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* CHAOS_LOG_H */
