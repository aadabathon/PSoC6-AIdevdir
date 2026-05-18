/**
 * @file    gpio_led.cpp
 */
#include "drivers/gpio_led.hpp"

namespace app {

GpioLed::GpioLed(cyhal_gpio_t pin) : pin_(pin) {}

GpioLed::~GpioLed() {
    if (initialized_) {
        cyhal_gpio_free(pin_);
    }
}

Status GpioLed::init() {
    // LEDs on this kit are active-LOW (writing 0 turns the LED on).
    // CYBSP_LED_STATE_OFF expands to the correct level for the BSP.
    cy_rslt_t r = cyhal_gpio_init(pin_,
                                  CYHAL_GPIO_DIR_OUTPUT,
                                  CYHAL_GPIO_DRIVE_STRONG,
                                  CYBSP_LED_STATE_OFF);
    if (r != CY_RSLT_SUCCESS) {
        return Status::HwError;
    }
    initialized_ = true;
    state_       = false;
    return Status::Ok;
}

void GpioLed::on() {
    if (!initialized_) return;
    cyhal_gpio_write(pin_, CYBSP_LED_STATE_ON);
    state_ = true;
}

void GpioLed::off() {
    if (!initialized_) return;
    cyhal_gpio_write(pin_, CYBSP_LED_STATE_OFF);
    state_ = false;
}

void GpioLed::toggle() {
    if (!initialized_) return;
    state_ = !state_;
    cyhal_gpio_write(pin_, state_ ? CYBSP_LED_STATE_ON : CYBSP_LED_STATE_OFF);
}

}  // namespace app
