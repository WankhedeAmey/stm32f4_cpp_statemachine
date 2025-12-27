# STM32F4 Event-Driven FSM (C++)

Firmware architecture implementation for the STM32F407G-DISC1. Demonstrates a C++ Finite State Machine (FSM) driven by hardware interrupts, utilizing the ARM Cortex-M ITM for low-overhead debugging.

## Architecture

### 1. C++ / HAL Bridge (`extern "C"`)
The STM32 HAL and startup routines are strictly C. To integrate C++ application logic without modifying vendor drivers, a "Bridge" pattern is used.
* **Problem:** C++ compilers "mangle" function names to support overloading (e.g., `void app_main()` becomes `_Z8app_mainv`), causing linker errors when called from C files.
* **Solution:** `extern "C"` forces standard C linkage, allowing `main.c` to call the C++ entry point and the C ISRs to trigger C++ object methods via a singleton pointer.

### 2. Zero-Copy Logging (ITM vs UART)
Standard UART logging was rejected to avoid blocking CPU cycles and external cabling dependencies.
* **Mechanism:** Uses the **Instrumentation Trace Macrocell (ITM)**, a hardware block inside the Cortex-M4.
* **Why:**
    * **Non-Blocking:** ITM writes to a hardware FIFO. Unlike UART, which can block for milliseconds (waiting for baud rate transmission), ITM consumes only a few CPU cycles.
    * **Hardware Efficient:** transmitted over the single SWO pin (PB3) already present in the ST-Link debug interface. No extra USB-TTL converter is required.

### 3. State Machine Logic
* **Design:** Deterministic FSM (`IDLE`, `ACTIVE`, `ERROR`).
* **Input:** External Interrupt (EXTI0) on PA0.
* **Output:** LED Status (PD12/PD14/PD15).
* **Concurrency:** Logic is decoupled from the ISR. The ISR sets a volatile atomic flag, and the main loop handles the state transition, keeping the interrupt context minimal.

## Hardware Setup
* **Board:** STM32F407G-DISC1
* **Core Clock:** 168 MHz (Critical for SWV Baud calculation)
* **Debug Interface:** ST-Link V2 (SWD + SWO)

## Usage
1. Open project in STM32CubeIDE.
2. **Critical Debug Setting:** In *Debug Configurations > Debugger*, enable **Serial Wire Viewer (SWV)** and set Core Clock to **168 MHz**.
3. Start Debug Session.
4. Open **SWV ITM Data Console**, enable **Port 0**, and click "Start Trace" (Red Button).
5. Resume code.

## License
MIT
