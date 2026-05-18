# hal_glue/

C-style bridge code. **The only place plain `.c` files belong in our tree.**

## Why this exists

The Infineon HAL is C. It expects C-linkage callbacks for ISRs and DMA events:

```c
void cyhal_gpio_register_callback(cyhal_gpio_t pin,
                                  cyhal_gpio_callback_data_t* cb_data);
```

C++ member functions cannot be passed directly as C callbacks (name mangling, hidden `this` parameter, etc.). The standard pattern is a free C function that forwards to a C++ method:

```c
// hal_glue/interrupt_handlers.c
extern void cpp_button_isr(void);  // C-linkage declared in C++ side

static cyhal_gpio_callback_data_t button_cb_data = {
    .callback = cpp_button_isr,
    .callback_arg = NULL,
};

void install_button_isr(void) {
    cyhal_gpio_register_callback(CYBSP_USER_BTN, &button_cb_data);
    cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL, 3, true);
}
```

```cpp
// drivers/button.cpp
extern "C" void cpp_button_isr(void) {
    Button::instance().on_press_from_isr();
}
```

The `extern "C"` tells the C++ compiler to emit the symbol with C linkage so the C side can find it.

## Rules

- One `.c` file per ISR group (button ISRs, DMA ISRs, etc.).
- Each `.c` file ONLY does the trampoline. No business logic.
- The corresponding C++ side exposes `extern "C"` functions for the trampolines to call.
- Keep ISRs short. Set a flag, give a semaphore, defer to a task.
