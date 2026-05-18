/**
 * @file    blink_task.cpp
 */
#include "tasks/blink_task.hpp"

#include "middleware/logger.hpp"
// #include "FreeRTOS.h"
// #include "task.h"

namespace app {

BlinkTask::BlinkTask(GpioLed& led) : led_(led) {}

Status BlinkTask::start() {
    // TODO once FreeRTOS is wired in:
    //
    // handle_ = xTaskCreateStatic(
    //     &BlinkTask::task_entry,
    //     "blink",
    //     kStackBlink,
    //     this,
    //     kPrioBlink,
    //     stack_,
    //     &tcb_);
    // return handle_ ? Status::Ok : Status::HwError;
    LOG_WARN("blink", "task not yet enabled (FreeRTOS not in build)");
    return Status::NotSupported;
}

void BlinkTask::task_entry(void* arg) {
    static_cast<BlinkTask*>(arg)->run();
}

void BlinkTask::run() {
    // TickType_t next_wake = xTaskGetTickCount();
    for (;;) {
        led_.toggle();
        // vTaskDelayUntil(&next_wake, pdMS_TO_TICKS(kBlinkPeriodMs));
    }
}

}  // namespace app
