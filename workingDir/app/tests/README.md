# tests/

Two flavors:

## `unit/` — host-runnable tests with mocked HAL

These compile with your laptop's compiler (clang or gcc, not `arm-none-eabi-`) against a tiny HAL mock that records calls and returns canned responses. Run via CMake or a simple Makefile under `tests/unit/`.

Useful for testing:
- Pure-logic middleware (`RingBuffer`, `Logger`, sensor fusion math)
- Driver state machines without real hardware
- Protocol parsing / packet construction

Frameworks worth considering:
- **GoogleTest** — heavy but excellent. Standard in industry.
- **Unity + CMock** — C-friendly, lightweight. Common in embedded shops.
- **Catch2** — header-only C++, easy to start with.

## `integration/` — hardware-in-the-loop tests

These build with `arm-none-eabi-gcc` like the main firmware, but the entry point is a test runner that exercises specific subsystems and reports pass/fail over UART. Useful for verifying things like "BMI270 actually responds at I2C address 0x68" that can't be checked on the host.

## What to test first

When you start writing drivers, write tests for `RingBuffer` first. It's the easiest one to get right and gives you confidence in your build setup. Then test the logic in any sensor driver that processes raw bytes into engineering units. Don't test the HAL itself — Infineon tests that.
