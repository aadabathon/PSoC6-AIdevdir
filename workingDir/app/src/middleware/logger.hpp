/**
 * @file    logger.hpp
 * @brief   printf-based logger with compile-time level filtering.
 *
 * Levels: ERR < WARN < INFO < DEBUG < TRACE.
 * Set LOG_LEVEL at build time (-DLOG_LEVEL=2) to compile out chatty levels.
 *   0 = OFF, 1 = ERR, 2 = WARN, 3 = INFO, 4 = DEBUG, 5 = TRACE
 *
 * Each macro prepends [LEVEL] [module] and appends "\r\n". Module name is
 * any short tag the caller passes; convention is the module that called it.
 *
 *   LOG_INFO("imu", "sample read: ax=%d ay=%d", s.ax, s.ay);
 *
 * Output goes wherever printf goes — i.e. retarget-io UART once UartConsole
 * has been initialized.
 */
#pragma once

#include <cstdio>

#ifndef LOG_LEVEL
#define LOG_LEVEL 3  // default = INFO
#endif

#define LOG_LEVEL_OFF   0
#define LOG_LEVEL_ERR   1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_INFO  3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_TRACE 5

#if LOG_LEVEL >= LOG_LEVEL_ERR
#define LOG_ERR(mod, fmt, ...) \
    std::printf("[ERR ] [%s] " fmt "\r\n", mod, ##__VA_ARGS__)
#else
#define LOG_ERR(mod, fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOG_WARN(mod, fmt, ...) \
    std::printf("[WARN] [%s] " fmt "\r\n", mod, ##__VA_ARGS__)
#else
#define LOG_WARN(mod, fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(mod, fmt, ...) \
    std::printf("[INFO] [%s] " fmt "\r\n", mod, ##__VA_ARGS__)
#else
#define LOG_INFO(mod, fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(mod, fmt, ...) \
    std::printf("[DBG ] [%s] " fmt "\r\n", mod, ##__VA_ARGS__)
#else
#define LOG_DEBUG(mod, fmt, ...) ((void)0)
#endif

#if LOG_LEVEL >= LOG_LEVEL_TRACE
#define LOG_TRACE(mod, fmt, ...) \
    std::printf("[TRC ] [%s] " fmt "\r\n", mod, ##__VA_ARGS__)
#else
#define LOG_TRACE(mod, fmt, ...) ((void)0)
#endif
