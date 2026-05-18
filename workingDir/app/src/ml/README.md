# ml/

DEEPCRAFT Studio exports drop here.

## Integration flow

1. Train your model in DEEPCRAFT Studio against data captured from this kit (via the streaming protocol firmware).
2. **Export → Embedded C model**. You get a folder with:
   - `imai.h`           ← API declarations
   - `imai.c`           ← inference engine
   - `imai_*.c`         ← model weights as `const` arrays
3. Copy those files into this directory.
4. The Makefile's `SOURCES+=$(wildcard $(CY_APP_PATH)/src/ml/*.c)` already picks them up — rebuild and they link in.

## Calling the model from C++

```cpp
#include "imai.h"

// In InferenceTask::run():
IMAI_init();

ImuSample s;
while (input_queue_.pop(s)) {
    float features[3] = { static_cast<float>(s.accel_x),
                          static_cast<float>(s.accel_y),
                          static_cast<float>(s.accel_z) };
    IMAI_enqueue(features);

    float output[NUM_CLASSES];
    if (IMAI_dequeue(output) == IMAI_RET_SUCCESS) {
        int predicted = argmax(output, NUM_CLASSES);
        output_queue_.push(predicted);
    }
}
```

The exact API (`IMAI_enqueue` / `IMAI_dequeue` vs. a single `IMAI_predict`) depends on whether your model is streaming or windowed. Check the exported `imai.h` header — it's well-commented.

## What NOT to commit

- The exported `.c`/`.h` files **can** be committed if the model is part of your repo's intent.
- If the weights are huge or proprietary, gitignore them and document the regen step.
- Either way, commit the DEEPCRAFT project file (`.imagimob`) somewhere so the model can be retrained.
