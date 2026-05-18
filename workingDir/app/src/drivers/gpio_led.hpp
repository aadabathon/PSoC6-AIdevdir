/**
 * @file    gpio_led.hpp
 * @brief   Output GPIO wrapped as an LED with on/off/toggle.
 *
 * Thin C++ wrapper over cyhal_gpio. Construction stores the pin; init()
 * configures the HAL. Destruction frees the pin.
 */
#pragma once

#include "app_config.hpp"
#include "cyhal.h"

namespace app {

class GpioLed {
public:
    /**
     * @brief Construct an LED wrapper. Does NOT init the HAL yet.
     * @param pin  BSP-defined GPIO pin (e.g. CYBSP_USER_LED).
     */
    explicit GpioLed(cyhal_gpio_t pin);

    ~GpioLed();

    GpioLed(const GpioLed&) = delete;
    GpioLed& operator=(const GpioLed&) = delete;

    /**
     * @brief Configure the pin as a push-pull output, initially off.
     */
    Status init();

    void on();
    void off();
    void toggle();
    bool is_on() const { return state_; }

private:
    cyhal_gpio_t pin_;
    bool initialized_ = false;
    bool state_       = false;
};

}  // namespace app
