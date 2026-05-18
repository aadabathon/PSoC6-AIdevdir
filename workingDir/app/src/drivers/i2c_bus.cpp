/**
 * @file    i2c_bus.cpp
 * @note    STUB. Wire these up to cyhal_i2c_* when you start sensor work.
 */
#include "drivers/i2c_bus.hpp"

namespace app {

I2cBus::I2cBus(cyhal_gpio_t sda, cyhal_gpio_t scl, uint32_t freq_hz)
    : sda_pin_(sda), scl_pin_(scl), freq_hz_(freq_hz) {}

I2cBus::~I2cBus() {
    if (initialized_) {
        cyhal_i2c_free(&handle_);
    }
}

Status I2cBus::init() {
    cy_rslt_t r = cyhal_i2c_init(&handle_, sda_pin_, scl_pin_, /*clk*/ nullptr);
    if (r != CY_RSLT_SUCCESS) return Status::HwError;

    cyhal_i2c_cfg_t cfg = {
        .is_slave      = false,
        .address       = 0,         // master has no address
        .frequencyhal_hz = freq_hz_,
    };
    r = cyhal_i2c_configure(&handle_, &cfg);
    if (r != CY_RSLT_SUCCESS) {
        cyhal_i2c_free(&handle_);
        return Status::HwError;
    }

    initialized_ = true;
    return Status::Ok;
}

Status I2cBus::write(uint8_t dev_addr, const uint8_t* data, size_t len) {
    if (!initialized_) return Status::NotInitialized;
    cy_rslt_t r = cyhal_i2c_master_write(&handle_, dev_addr,
                                         data, len,
                                         /*timeout_ms*/ 100,
                                         /*send_stop*/  true);
    return from_cy_rslt(r);
}

Status I2cBus::read(uint8_t dev_addr, uint8_t* data, size_t len) {
    if (!initialized_) return Status::NotInitialized;
    cy_rslt_t r = cyhal_i2c_master_read(&handle_, dev_addr,
                                        data, len,
                                        /*timeout_ms*/ 100,
                                        /*send_stop*/  true);
    return from_cy_rslt(r);
}

Status I2cBus::read_register(uint8_t dev_addr, uint8_t reg,
                             uint8_t* data, size_t len) {
    if (!initialized_) return Status::NotInitialized;
    cy_rslt_t r = cyhal_i2c_master_mem_read(&handle_, dev_addr,
                                            reg, /*memaddr_size*/ 1,
                                            data, len,
                                            /*timeout_ms*/ 100);
    return from_cy_rslt(r);
}

Status I2cBus::write_register(uint8_t dev_addr, uint8_t reg, uint8_t value) {
    if (!initialized_) return Status::NotInitialized;
    cy_rslt_t r = cyhal_i2c_master_mem_write(&handle_, dev_addr,
                                             reg, /*memaddr_size*/ 1,
                                             &value, 1,
                                             /*timeout_ms*/ 100);
    return from_cy_rslt(r);
}

}  // namespace app
