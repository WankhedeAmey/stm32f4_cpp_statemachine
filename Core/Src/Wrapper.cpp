#include "Wrapper.h"
#include "SystemController.h"

// Global pointer to our class for ISR access
SystemController* g_sysPointer = nullptr;

// The standard HAL callback in C
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    // Check if interrupt src is PA0 (User Button)
    if (GPIO_Pin == GPIO_PIN_0) {
        // Debounce check for 200ms
        static uint32_t last_tick = 0;
        if (HAL_GetTick() - last_tick > 200) {

            // Call the C++ object if it exists
            if (g_sysPointer != nullptr) {
            	g_sysPointer->onUserButtonPress();
            }

            last_tick = HAL_GetTick();
        }
    }
}

void cpp_main() {
    SystemController sysInstance;

    g_sysPointer = &sysInstance;

    sysInstance.run();
}
