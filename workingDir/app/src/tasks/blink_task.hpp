/**
 * @file    blink_task.hpp
 * @brief   Heartbeat task — toggles an LED every kBlinkPeriodMs to prove
 *          the RTOS is alive. The "are we breathing" indicator.
 *
 * STUB. Uncomment FreeRTOS includes and the create call once you switch
 * System::start() from the spin loop to vTaskStartScheduler().
 */
#pragma once

#include "app_config.hpp"
#include "drivers/gpio_led.hpp"
// #include "FreeRTOS.h"
// #include "task.h"

namespace app {

class BlinkTask {
public:
    explicit BlinkTask(GpioLed& led);

    Status start();

private:
    static void task_entry(void* arg);
    void run();

    GpioLed& led_;
    // TaskHandle_t  handle_ = nullptr;
    // StaticTask_t  tcb_;
    // StackType_t   stack_[kStackBlink];
};

}  // namespace app
