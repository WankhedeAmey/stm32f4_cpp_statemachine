#ifndef INC_SYSTEMCONTROLLER_H_
#define INC_SYSTEMCONTROLLER_H_
#include "main.h"

enum class State {
	IDLE,
	ACTIVE,
	ERROR
};

class SystemController {
public:
	SystemController();
	void run();
	void onUserButtonPress();
private:
	State currentState;
	// the flag for the Interrupt
	volatile bool buttonPressedFlag;

	// Internal State Handlers
	void handleIdle();
	void handleActive();
	void handleError();

	// Helper to simulate work
	void processWorkload();

};

#endif /* INC_SYSTEMCONTROLLER_H_ */
