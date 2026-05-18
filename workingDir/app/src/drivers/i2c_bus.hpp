/**
 * @file    i2c_bus.hpp
 * @brief   I2C master bus shared by multiple sensor drivers.
 *
 * Sensor drivers (BMI270, BMM350, DPS368) take an I2cBus& at construction.
 * They issue register reads/writes through it. Concurrent access from
 * multiple tasks requires external synchronization (see middleware/).
 *
 * NOTE: This is a stub. Fill in the implementation when you start writing
 * the sensor drivers. The shape of the API below is what the Bosch
 * SensorAPI expects as its bus-access callbacks.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "app_config.hpp"
#include "cyhal.h"

namespace app {

class I2cBus {
public:
    /**
     * @param sda  SDA pin (e.g. kSensorI2cSda)
     * @param scl  SCL pin (e.g. kSensorI2cScl)
     * @param freq_hz  bus frequency, typically 100 kHz or 400 kHz
     */
    I2cBus(cyhal_gpio_t sda, cyhal_gpio_t scl, uint32_t freq_hz);
    ~I2cBus();

    I2cBus(const I2cBus&) = delete;
    I2cBus& operator=(const I2cBus&) = delete;

    Status init();

    /**
     * @brief Write `len` bytes from `data` to a 7-bit I2C device.
     */
    Status write(uint8_t dev_addr, const uint8_t* data, size_t len);

    /**
     * @brief Read `len` bytes from a 7-bit I2C device into `data`.
     */
    Status read(uint8_t dev_addr, uint8_t* data, size_t len);

    /**
     * @brief Write a register address, then read `len` bytes back. The
     *        typical pattern for sensor register reads.
     */
    Status read_register(uint8_t dev_addr, uint8_t reg, uint8_t* data, size_t len);

    /**
     * @brief Write a single register value. Convenience for the common case.
     */
    Status write_register(uint8_t dev_addr, uint8_t reg, uint8_t value);

private:
    cyhal_i2c_t  handle_{};
    cyhal_gpio_t sda_pin_;
    cyhal_gpio_t scl_pin_;
    uint32_t     freq_hz_;
    bool         initialized_ = false;
};

}  // namespace app
