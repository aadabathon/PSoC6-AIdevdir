# Driver conventions

Every driver in `app/src/drivers/` follows these rules. Consistency means new drivers are fast to write and easy to review.

## File layout

One peripheral or sensor per pair of files:

```
drivers/foo.hpp   ← class declaration, public API
drivers/foo.cpp   ← implementation
```

For multi-instance peripherals (e.g. two I²C buses), the class is parameterized at construction:

```cpp
I2cBus bus0(CYBSP_I2C_SDA, CYBSP_I2C_SCL);
I2cBus bus1(P10_0, P10_1);
```

## Class shape

```cpp
class FooDriver {
public:
    // Construction acquires the hardware resource.
    FooDriver(/* pins, parents, config */);

    // Destruction releases it.
    ~FooDriver();

    // Drivers own hardware — non-copyable, non-movable by default.
    FooDriver(const FooDriver&) = delete;
    FooDriver& operator=(const FooDriver&) = delete;

    // init() runs after construction for anything that can fail.
    // Constructor only does what cannot fail.
    Status init();

    // Public operations return Status; data via out-params or struct returns.
    Status read(SomeData& out);
    Status write(const SomeData& in);

private:
    // Underlying HAL handle as a member.
    cyhal_foo_t handle_;
    // Other internal state (cached config, flags, ...).
    bool initialized_ = false;
};
```

## Rules

1. **RAII.** Constructor acquires, destructor releases. If acquisition can fail, do it in `init()` and have the destructor check `initialized_` before releasing.
2. **No exceptions.** Return `Status`. Caller checks.
3. **No logging from drivers.** Return errors; let the task or middleware log them. Drivers stay testable without a logger.
4. **No FreeRTOS calls.** No `xQueueSend`, no `vTaskDelay`. If a driver needs to block, use `cyhal_system_delay_ms` (HAL-only) or accept a timeout parameter that the caller honors.
5. **No global state.** Singletons are tasks' problem, not drivers'.
6. **Thread safety is NOT a driver concern by default.** Document whether the driver is safe for concurrent use. Most aren't — wrap with a mutex at a higher layer if needed.

## `Status` enum

Defined in `include/app_config.hpp`:

```cpp
enum class Status : uint8_t {
    Ok = 0,
    BadParam,
    Timeout,
    HwError,
    NotInitialized,
    NotSupported,
};
```

Convert from HAL:

```cpp
inline Status from_cy_rslt(cy_rslt_t r) {
    return (r == CY_RSLT_SUCCESS) ? Status::Ok : Status::HwError;
}
```

## Testing

Each driver should have a host-side unit test in `tests/unit/test_<name>.cpp` that uses a mocked HAL. See `tests/README.md` for the mocking strategy.
