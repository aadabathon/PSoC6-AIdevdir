/**
 * @file    bmi270_driver.cpp
 * @note    STUB. Fill in once Bosch SensorAPI is vendored.
 */
#include "drivers/bmi270_driver.hpp"

namespace app {

Bmi270Driver::Bmi270Driver(I2cBus& bus, uint8_t addr)
    : bus_(bus), addr_(addr) {}

Status Bmi270Driver::init() {
    // TODO:
    //   - Verify chip ID by reading register 0x00 (should be 0x24 for BMI270).
    //   - Call bmi2_init() from the Bosch SensorAPI.
    //   - Configure accel: ODR 50 Hz, range ±4g, filter BWP_NORMAL.
    //   - Configure gyro:  ODR 50 Hz, range ±2000 dps.
    //   - Enable both sensors via bmi2_sensor_enable().
    //
    // For now, just probe the I2C address to confirm wiring.
    uint8_t chip_id = 0;
    Status s = bus_.read_register(addr_, /*reg CHIP_ID*/ 0x00, &chip_id, 1);
    if (s != Status::Ok) return s;
    if (chip_id != 0x24) return Status::HwError;  // wrong device on this address

    initialized_ = true;
    return Status::Ok;
}

Status Bmi270Driver::read_sample(ImuSample& out) {
    if (!initialized_) return Status::NotInitialized;

    // TODO: replace with bmi2_get_sensor_data(); for now return a stub.
    out = {};
    return Status::NotSupported;
}

}  // namespace app
