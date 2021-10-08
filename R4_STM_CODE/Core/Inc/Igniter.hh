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
public:
	/* Constructor */
	Igniter(GPIO_TypeDef* _GPIO_PORT_IGNITER, uint16_t _PIN_IGNITER,
			GPIO_TypeDef* _GPIO_PORT_TEST_CON, uint16_t _PIN_TEST_CON);

	/* Methods */
	bool isConnected() const;
	void FIRE(uint16_t milisecs = 50);
};

#endif /* INC_IGNITER_HH_ */
