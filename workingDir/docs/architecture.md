# Architecture

## System overview

This firmware targets the **Infineon CY8CKIT-062S2-AI** (PSoC 62 MCU, dual-core: Cortex-M4F + Cortex-M0+). Only the **CM4 core** is used for application code; the CM0+ runs the default Infineon boot/sleep firmware.

The system collects motion and audio data from onboard sensors, runs on-device ML inference (model exported from DEEPCRAFT Studio), and reports results over UART.

## Layer diagram

```
┌─────────────────────────────────────────────────────┐
│  Application                                        │
│    main.cpp → System::start()                       │
└─────────────────────────────────────────────────────┘
                       │
┌─────────────────────────────────────────────────────┐
│  Tasks (FreeRTOS)                                   │
│    ImuTask  InferenceTask  TelemetryTask  UiTask    │
└─────────────────────────────────────────────────────┘
                       │
┌─────────────────────────────────────────────────────┐
│  Middleware                                         │
│    Logger  RingBuffer<T>  SensorFusion              │
└─────────────────────────────────────────────────────┘
                       │
┌─────────────────────────────────────────────────────┐
│  Drivers (C++ wrappers, RAII)                       │
│    GpioLed  UartConsole  I2cBus  SpiBus             │
│    Bmi270Driver  Bmm350Driver  Dps368Driver         │
└─────────────────────────────────────────────────────┘
                       │
┌─────────────────────────────────────────────────────┐
│  Infineon HAL (C)                                   │
│    cyhal_gpio_*  cyhal_i2c_*  cyhal_uart_*  ...     │
└─────────────────────────────────────────────────────┘
                       │
┌─────────────────────────────────────────────────────┐
│  PDL / register access                              │
└─────────────────────────────────────────────────────┘
                       │
                  Silicon (PSoC 62)
```

## Dependency rules

- **Drivers** depend on the HAL only. They **never** call FreeRTOS, log, or touch other drivers' state.
- **Middleware** is hardware-agnostic. Header-only or HAL-free where possible.
- **Tasks** orchestrate: they own driver instances, push to queues, read from queues, log status. Tasks **never** call other tasks directly — they communicate via queues / event groups / notifications.
- **`hal_glue/`** is the *only* place C-style ISR functions live. They translate HAL callbacks into method calls on C++ objects.

## Memory plan

| Region | Size  | Use                                            |
|--------|-------|------------------------------------------------|
| Flash  | 2 MB  | Code, const data, ML model weights             |
| SRAM   | 1 MB  | Stacks, heap (minimal), DMA buffers, RTOS bss  |

- **No dynamic allocation after init.** All RTOS objects (tasks, queues, mutexes) are statically allocated.
- ML model weights are `const` in flash; only activation tensors live in SRAM.
- DMA buffers for PDM mic audio are placed in a dedicated SRAM section (see linker script).

## Boot sequence

1. PSoC boot ROM → vector table → `Reset_Handler` (in startup `.S` from BSP).
2. Reset handler: copy `.data` from flash, zero `.bss`, run C++ static constructors (`__libc_init_array`).
3. `main()` in `main.cpp` → constructs `System` object on the stack.
4. `system.init()`: brings up clocks via `cybsp_init()`, initializes drivers in dependency order.
5. `system.start()`: creates FreeRTOS tasks, calls `vTaskStartScheduler()`. **Does not return.**

## Failure handling

- HAL functions return `cy_rslt_t`. Wrappers translate to our `Status` enum.
- Init-time failures: log + halt (`CY_ASSERT(0)`).
- Runtime failures: log + degrade gracefully (e.g., drop a sample, retry, fall back).
- Hard faults: caught by `HardFault_Handler`, dumps registers over UART, then resets.
