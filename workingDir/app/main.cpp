/**
 * @file    main.cpp
 * @brief   Application entry point.
 * @author Adam Shebani (adamsheb414@gmail.com)
 */
#include "system.hpp"

int main(void) {
    app::System system;

    if (system.init() != app::Status::Ok) {
        // Already logged / halted by init() if reached here something is wrong.
        while (true) {}
    }

    system.start();  // [[noreturn]]
}
