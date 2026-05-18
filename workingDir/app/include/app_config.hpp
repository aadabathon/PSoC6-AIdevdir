/**
 * @file    app_config.hpp
 * @brief   Compile-time configuration for the entire application.
 *
 * Everything tunable lives here: pins, periods, buffer sizes, task stacks.
 * If you find yourself hardcoding a magic number elsewhere, move it here.
 */
#pragma once

#include <cstdint>

#include "cybsp.h"
#include "cyhal.h"

namespace app {

// ============================================================================
// Pin assignments
// ============================================================================
// Aliased from BSP defaults so we can override in one place if needed.
// (For example, when porting to a custom board with different pinout.)
constexpr cyhal_gpio_t kUserLedPin    = CYBSP_USER_LED;
constexpr cyhal_gpio_t kUserLed2Pin   = CYBSP_USER_LED2;
constexpr cyhal_gpio_t kUserBtnPin    = CYBSP_USER_BTN;

constexpr cyhal_gpio_t kDebugUartTx   = CYBSP_DEBUG_UART_TX;
constexpr cyhal_gpio_t kDebugUartRx   = CYBSP_DEBUG_UART_RX;

constexpr cyhal_gpio_t kSensorI2cSda  = CYBSP_I2C_SDA;
constexpr cyhal_gpio_t kSensorI2cScl  = CYBSP_I2C_SCL;

// ============================================================================
// UART
// ============================================================================
constexpr uint32_t kDebugUartBaud = 115200u;

// ============================================================================
// I2C
// ============================================================================
constexpr uint32_t kSensorI2cHz = 400000u;  // 400 kHz fast-mode

// ============================================================================
// Sensor I2C addresses (7-bit)
// ============================================================================
constexpr uint8_t kBmi270Addr = 0x68;   // SDO=GND
constexpr uint8_t kBmm350Addr = 0x14;   // CSB=GND
constexpr uint8_t kDps368Addr = 0x77;   // SDO=VDD

// ============================================================================
// Task priorities (higher = more important)
// ============================================================================
constexpr uint8_t kPrioIdle      = 0;
constexpr uint8_t kPrioBlink     = 1;
constexpr uint8_t kPrioTelemetry = 2;
constexpr uint8_t kPrioUi        = 3;
constexpr uint8_t kPrioInference = 4;
constexpr uint8_t kPrioImu       = 5;

// ============================================================================
// Task stack sizes (in WORDS — multiply by 4 for bytes on Cortex-M4)
// ============================================================================
constexpr uint16_t kStackBlink     = 256;
constexpr uint16_t kStackTelemetry = 512;
constexpr uint16_t kStackUi        = 512;
constexpr uint16_t kStackImu       = 1024;
constexpr uint16_t kStackInference = 4096;

// ============================================================================
// Periods (ms)
// ============================================================================
constexpr uint32_t kBlinkPeriodMs  = 500;
constexpr uint32_t kImuPeriodMs    = 20;    // 50 Hz

// ============================================================================
// Buffer sizes
// ============================================================================
constexpr size_t kImuQueueDepth      = 64;   // samples
constexpr size_t kLogRingBufferBytes = 4096;
constexpr size_t kMlWindowSamples    = 100;  // 2 s @ 50 Hz

// ============================================================================
// Status codes — used everywhere as the return type for fallible operations.
// ============================================================================
enum class Status : uint8_t {
    Ok = 0,
    BadParam,
    Timeout,
    HwError,
    NotInitialized,
    NotSupported,
};

/**
 * @brief Convert an Infineon HAL result to our Status enum.
 */
inline Status from_cy_rslt(cy_rslt_t r) {
    return (r == CY_RSLT_SUCCESS) ? Status::Ok : Status::HwError;
}

/**
 * @brief Stringify Status for logging.
 */
inline const char* to_string(Status s) {
    switch (s) {
        case Status::Ok:             return "Ok";
        case Status::BadParam:       return "BadParam";
        case Status::Timeout:        return "Timeout";
        case Status::HwError:        return "HwError";
        case Status::NotInitialized: return "NotInitialized";
        case Status::NotSupported:   return "NotSupported";
    }
    return "Unknown";
}

}  // namespace app
