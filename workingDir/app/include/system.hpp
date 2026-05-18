/**
 * @file    system.hpp
 * @brief   Top-level system orchestrator.
 *
 * The System object owns every driver and every task as members. This makes
 * the boot order explicit (read top-to-bottom) and ties all lifetimes to one
 * place. main() constructs a single System, calls init() then start(), and
 * start() never returns (it hands control to the FreeRTOS scheduler).
 */
#pragma once

#include "app_config.hpp"
#include "drivers/gpio_led.hpp"
#include "drivers/uart_console.hpp"
// Add more drivers as you implement them:
// #include "drivers/i2c_bus.hpp"
// #include "drivers/bmi270_driver.hpp"
//
// Add tasks as you implement them:
// #include "tasks/blink_task.hpp"
// #include "tasks/imu_task.hpp"

namespace app {

class System {
public:
    System();
    ~System() = default;

    // System owns hardware — non-copyable.
    System(const System&) = delete;
    System& operator=(const System&) = delete;

    /**
     * @brief Bring up clocks, init all drivers in dependency order.
     *        Called before the scheduler starts.
     */
    Status init();

    /**
     * @brief Create all FreeRTOS tasks and start the scheduler.
     *        Does not return on success.
     */
    [[noreturn]] void start();

private:
    // --- Drivers ---
    // Order matters: declared in init order, destroyed in reverse.
    GpioLed      heartbeat_led_;
    UartConsole  console_;

    // --- Tasks (uncomment as you add them) ---
    // BlinkTask blink_task_;
    // ImuTask   imu_task_;
};

}  // namespace app
