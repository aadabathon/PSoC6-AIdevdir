/**
 * @file    bmi270_driver.hpp
 * @brief   Driver for Bosch BMI270 6-axis IMU.
 *
 * STUB. The Bosch SensorAPI does the heavy lifting:
 *   third_party/BMI270-Sensor-API/
 * You provide read/write/delay callbacks that bridge to our I2cBus, then
 * call the Bosch top-level functions for init, config, and reads.
 *
 * Recommended flow for filling this in:
 *   1. Add Bosch BMI270-Sensor-API as a submodule under third_party/.
 *   2. Add its .c files to SOURCES in the Makefile fragment.
 *   3. Implement the three callbacks (read/write/delay_us) as free functions
 *      in this .cpp that forward to a static I2cBus* and cyhal_system_delay.
 *   4. In init(), call bmi2_init(), then configure accel + gyro features.
 *   5. read_sample() calls bmi2_get_sensor_data().
 */
#pragma once

#include <cstdint>

#include "app_config.hpp"
#include "drivers/i2c_bus.hpp"

namespace app {

struct ImuSample {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    uint32_t timestamp_ms;
};

class Bmi270Driver {
public:
    Bmi270Driver(I2cBus& bus, uint8_t addr = kBmi270Addr);
    ~Bmi270Driver() = default;

    Bmi270Driver(const Bmi270Driver&) = delete;
    Bmi270Driver& operator=(const Bmi270Driver&) = delete;

    Status init();
    Status read_sample(ImuSample& out);

private:
    I2cBus& bus_;
    uint8_t addr_;
    bool    initialized_ = false;
};

}  // namespace app
