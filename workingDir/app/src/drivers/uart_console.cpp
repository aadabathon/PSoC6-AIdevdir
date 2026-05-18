/**
 * @file    uart_console.cpp
 */
#include "drivers/uart_console.hpp"

#include "cy_retarget_io.h"

namespace app {

UartConsole::UartConsole(cyhal_gpio_t tx, cyhal_gpio_t rx, uint32_t baud)
    : tx_pin_(tx), rx_pin_(rx), baud_(baud) {}

UartConsole::~UartConsole() {
    if (initialized_) {
        cy_retarget_io_deinit();
    }
}

Status UartConsole::init() {
    // NOTE: use the non-flow-control variant. The KitProg3 UART bridge only
    // connects TX/RX, not CTS/RTS. Calling cy_retarget_io_init_fc() on this
    // board will fail and hang the boot inside a CY_ASSERT(0). We learned
    // this the hard way — see git log around the blink debug session.
    cy_rslt_t r = cy_retarget_io_init(tx_pin_, rx_pin_, baud_);
    if (r != CY_RSLT_SUCCESS) {
        return Status::HwError;
    }
    initialized_ = true;
    return Status::Ok;
}

Status UartConsole::read_char(uint8_t& out, uint32_t timeout_ms) {
    if (!initialized_) return Status::NotInitialized;

    cy_rslt_t r = cyhal_uart_getc(&cy_retarget_io_uart_obj, &out,
                                  timeout_ms == 0 ? 1 : timeout_ms);
    if (r == CY_RSLT_SUCCESS) return Status::Ok;
    // The HAL distinguishes "nothing available" from real errors; treat
    // both as Timeout for the caller's purposes — they retry the same way.
    return Status::Timeout;
}

}  // namespace app
