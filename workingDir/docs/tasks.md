# Tasks

FreeRTOS tasks live in `app/src/tasks/`. Each task is a class that wraps a FreeRTOS task handle.

## Task table

| Task            | Priority | Stack (words) | Period      | Purpose                                  |
|-----------------|----------|---------------|-------------|------------------------------------------|
| `IdleTask`      | 0        | 256           | always      | FreeRTOS built-in idle hook              |
| `BlinkTask`     | 1        | 256           | 500 ms      | Heartbeat LED (proves the RTOS is alive) |
| `TelemetryTask` | 2        | 512           | event       | UART log drain                           |
| `UiTask`        | 3        | 512           | event       | Button input, LED status                 |
| `ImuTask`       | 5        | 1024          | 20 ms       | Read BMI270, push to queue               |
| `InferenceTask` | 4        | 4096          | event       | Run ML model on IMU window               |

Priorities: higher number = higher priority. Numbers chosen to be `< configMAX_PRIORITIES` (default 7 in FreeRTOSConfig).

## Task class shape

```cpp
class FooTask {
public:
    explicit FooTask(/* dependencies as references */);
    Status start();  // creates the FreeRTOS task

private:
    static void task_entry(void* arg);  // C-compatible thunk
    void run();                          // the actual task loop

    TaskHandle_t handle_ = nullptr;
    StaticTask_t tcb_;                   // static allocation
    StackType_t stack_[FOO_STACK_WORDS];
};
```

## Rules

1. **Tasks own drivers via reference, not value.** The `System` object owns drivers; tasks borrow them. This makes lifetimes explicit and unit tests easier.
2. **Communicate via queues/notifications, never shared globals.** If two tasks need to share state, route it through a queue.
3. **No driver init in tasks.** All hardware init happens in `System::init()` before the scheduler starts.
4. **Static allocation only.** `xTaskCreateStatic`, not `xTaskCreate`. No dynamic memory in the RTOS layer.
5. **Period via `vTaskDelayUntil`, not `vTaskDelay`.** `DelayUntil` accounts for the time the task spent running. Important for periodic sensor reads.

## Inter-task communication

```
        IMU @50Hz
            │
            ▼
   ┌─────────────────┐    imu_queue    ┌─────────────────┐
   │    ImuTask      ├────────────────▶│ InferenceTask   │
   └─────────────────┘  (vec3 samples) └────────┬────────┘
                                                │ predictions
                                                ▼
                                        ┌──────────────┐
                                        │TelemetryTask │ → UART
                                        └──────────────┘
```

Queues are sized to absorb ~100 ms of jitter without dropping samples.
