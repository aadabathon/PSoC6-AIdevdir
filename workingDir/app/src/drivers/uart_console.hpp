/**
 * @file    uart_console.hpp
 * @brief   Debug UART wrapped with retarget-io so printf "just works".
 *
 * After init(), stdout is redirected to this UART by retarget-io. From then
 * on, printf() / fputs() / std::cout all go out the KitProg3 USB-UART bridge
 * to your host terminal at the configured baud.
 */
#pragma once

#include "app_config.hpp"
#include "cyhal.h"

namespace app {

class UartConsole {
public:
    UartConsole(cyhal_gpio_t tx, cyhal_gpio_t rx, uint32_t baud);
    ~UartConsole();

    UartConsole(const UartConsole&) = delete;
    UartConsole& operator=(const UartConsole&) = delete;

    /**
     * @brief Initialize HAL UART and hook up retarget-io.
     *        After this returns Ok, printf() works.
     */
    Status init();

    /**
     * @brief Read a single character non-blocking-ish.
     * @param out  receives the character on success.
     * @param timeout_ms  0 = non-blocking.
     */
    Status read_char(uint8_t& out, uint32_t timeout_ms = 0);

private:
    cyhal_gpio_t tx_pin_;
    cyhal_gpio_t rx_pin_;
    uint32_t     baud_;
    bool         initialized_ = false;
};

}  // namespace app
