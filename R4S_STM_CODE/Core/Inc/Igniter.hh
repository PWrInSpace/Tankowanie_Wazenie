#ifndef INC_IGNITER_HH_
#define INC_IGNITER_HH_

#include "gpio.h"
#include "stdbool.h"
#include "stdlib.h"

/* Struktura zapalnika */


typedef struct Igniter{
	// Igniter Pin
	GPIO_TypeDef* GPIO_PORT_IGNITER;
	uint16_t PIN_IGNITER;
	// Test Connection
	GPIO_TypeDef* GPIO_PORT_TEST_CON;
	uint16_t PIN_TEST_CON;

}Igniter;

/* Konstruktor */
Igniter* igniter_init(GPIO_TypeDef* _GPIO_PORT_IGNITER, uint16_t _PIN_IGNITER, GPIO_TypeDef* _GPIO_PORT_TEST_CON, uint16_t _PIN_TEST_CON);

bool igniter_is_connected(Igniter* Igniter);
bool igniter_FIRE(Igniter* Igniter);

#endif /* INC_IGNITER_HH_ */
