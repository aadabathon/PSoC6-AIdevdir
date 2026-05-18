/**
 * @file    system.cpp
 * @brief   System bring-up implementation.
 */
#include "system.hpp"

#include <cstdio>

#include "cybsp.h"
#include "cyhal.h"
#include "version.hpp"

namespace app {

System::System()
    : heartbeat_led_(kUserLedPin),
      console_(kDebugUartTx, kDebugUartRx, kDebugUartBaud) {
    // Constructors only do what cannot fail. All fallible setup is in init().
}

Status System::init() {
    // 1. Board init: clocks, power, default pin configuration.
    cy_rslt_t r = cybsp_init();
    if (r != CY_RSLT_SUCCESS) {
        // No console yet — halt.
        CY_ASSERT(0);
        return Status::HwError;
    }

    // 2. Enable global interrupts. Drivers may register ISRs after this.
    __enable_irq();

    // 3. Console comes up first so subsequent failures can be reported.
    Status s = console_.init();
    if (s != Status::Ok) {
        CY_ASSERT(0);
        return s;
    }

    // 4. Print banner.
    printf("\x1b[2J\x1b[;H");  // ANSI clear screen
    printf("===========================================\r\n");
    printf("  PSoC6-AI Firmware  v%s\r\n", kVersionString);
    printf("  build: %s  git: %s\r\n", kBuildDate, kVersionGitHash);
    printf("===========================================\r\n");

    // 5. Initialize remaining drivers in dependency order.
    s = heartbeat_led_.init();
    if (s != Status::Ok) {
        printf("[FATAL] heartbeat LED init: %s\r\n", to_string(s));
        return s;
    }

    // (As you add drivers, init them here, log each one.)

    printf("[INFO] system init complete\r\n");
    return Status::Ok;
}

[[noreturn]] void System::start() {
    // (As you add tasks, call task.start() on each here.)
    // blink_task_.start();
    // imu_task_.start();

    printf("[INFO] starting scheduler\r\n");

    // For now, with no tasks created, just spin so you can see life from main.
    // Once you add tasks, replace this with vTaskStartScheduler().
    for (;;) {
        heartbeat_led_.toggle();
        cyhal_system_delay_ms(kBlinkPeriodMs);
    }

    // When you switch to RTOS:
    // vTaskStartScheduler();
    // CY_ASSERT(0);  // scheduler should never return
    // while (true) {}
}

}  // namespace app
