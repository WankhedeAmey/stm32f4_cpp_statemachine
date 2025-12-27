#include "SystemController.h"
#include <cstdio>

SystemController::SystemController() : currentState(State::IDLE), buttonPressedFlag(false) {
    printf("(System) : Initialized. Waiting for input...\n");
}

void SystemController::run() {
    // state machine loop
    while (true) {
        switch (currentState) {
            case State::IDLE:
                handleIdle();
                break;
            case State::ACTIVE:
                handleActive();
                break;
            case State::ERROR:
                handleError();
                break;
        }
        // delay to prevent the log flooding
        HAL_Delay(100);
    }
}

void SystemController::onUserButtonPress() {
	// This runs fast as it is part of ISR
	buttonPressedFlag = true;
}

void SystemController::handleIdle() {

//    bool buttonPressed = false;
//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
//    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
//    	buttonPressed = true;
//    }
//
//    if (buttonPressed) {
//    	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
//        printf("[Transition] IDLE -> ACTIVE\n");
//        currentState = State::ACTIVE;
//
//        // Debounce
//        HAL_Delay(300);
//    }
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
    if (buttonPressedFlag) {
    	buttonPressedFlag = false;
    	printf("(Transition) : IDLE -> ACTIVE via EXTI \n");
    	currentState = State::ACTIVE;
    	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
    }
}

void SystemController::handleActive() {
    static int workCounter = 0;

    // toggle green LED to indicate handling Active state
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
    processWorkload();
    workCounter++;

    if (workCounter >= 10) {
        // Simulate a fault after 10 cycles
        printf("(Transition) : ACTIVE -> ERROR (Simulated Fault)\n");
        currentState = State::ERROR;
        workCounter = 0;
    }
}

void SystemController::handleError() {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

    printf("(State) : ERROR. System Halted. Resetting in 3s...\n");

    HAL_Delay(3000);

    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);

    printf("(Transition) : ERROR -> IDLE\n");
    currentState = State::IDLE;
}

void SystemController::processWorkload() {
    // Dummy payload
    printf("Processing...\n");
}
