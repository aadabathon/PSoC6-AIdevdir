# PSoC6-ai-board

Firmware development for the Infineon CY8CKIT-062S2-AI evaluation kit.

## Layout

```
PSoC6-ai-board/
├── docs/              Architecture notes, driver conventions, datasheets
├── app/               The ModusToolbox application
│   ├── include/         Public headers (app-wide config, system)
│   ├── src/
│   │   ├── drivers/     One C++ wrapper per peripheral (HAL-backed)
│   │   ├── tasks/       FreeRTOS tasks (policy / orchestration)
│   │   ├── middleware/  Reusable non-driver utilities (ring buffer, logger)
│   │   ├── ml/          DEEPCRAFT-exported model lands here
│   │   └── hal_glue/    C↔C++ glue (ISRs that dispatch into C++ objects)
│   └── tests/
│       ├── unit/        Host-runnable tests with mocked HAL
│       └── integration/ Hardware-in-the-loop tests
├── tools/             Helper scripts (formatting, version stamping, log capture)
└── third_party/       Vendored libraries not pulled by ModusToolbox (Bosch SensorAPIs, ETL)
```

## Setup (first time on a new machine)

1. Install **ModusToolbox 3.6+** from the [Infineon installer](https://softwaretools.infineon.com/tools/com.ifx.tb.tool.modustoolboxsetup).
2. Open **ModusToolbox Shell** in this directory.
3. From `app/`, generate the ModusToolbox project skeleton alongside the existing source tree:
   ```bash
   cd app
   project-creator-cli \
     --board-id CY8CKIT-062S2-AI \
     --app-id mtb-example-hal-hello-world \
     --user-app-name psoc6_ai \
     --target-dir .
   ```
   This drops `Makefile`, `deps/`, `bsps/`, and a `main.c` next to our `src/` tree.
4. Delete the generated `main.c` (we use `main.cpp` from `src/`).
5. Apply our Makefile additions — see `app/Makefile.local.mk` and append its contents to the generated `Makefile`.
6. Pull dependencies:
   ```bash
   make getlibs
   ```
7. Build and flash:
   ```bash
   make build -j8
   make program
   ```
8. Open a serial terminal at **115200 8-N-1** on the KitProg3 COM port. You should see boot output and the LED blinking at 1 Hz.

## VS Code workflow

Install the **ModusToolbox Assistant**, **C/C++** (Microsoft), and **Cortex-Debug** (marus25) extensions. Then from the project root:

```bash
cd app
make vscode
code ..
```

This generates `.vscode/` configs in `app/`. F5 launches a debug session via KitProg3.

## Coding conventions

See `docs/drivers.md` for the driver API conventions and `docs/tasks.md` for the RTOS task layout. Code style is enforced by `.clang-format` at the repo root. Run `tools/format.sh` before committing.
