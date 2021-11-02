#ifndef INC_IGNITER_HH_
#define INC_IGNITER_HH_

#include "gpio.h"
#include "stdbool.h"
#include "stdlib.h"

class Igniter{
	// Igniter Pin
	GPIO_TypeDef* GPIO_PORT_IGNITER;
	uint16_t PIN_IGNITER;
	// Test Connection
	GPIO_TypeDef* GPIO_PORT_TEST_CON;
	uint16_t PIN_TEST_CON;
	bool tried;
public:
	/* Constructor */
	Igniter(GPIO_TypeDef* GPIO_PORT_IGNITER_, uint16_t PIN_IGNITER_,
			GPIO_TypeDef* GPIO_PORT_TEST_CON_, uint16_t PIN_TEST_CON_);

	/* Methods */
	uint8_t GetStatus() const;
	void FIRE(uint16_t Milisecs = 50);
};

#endif /* INC_IGNITER_HH_ */
