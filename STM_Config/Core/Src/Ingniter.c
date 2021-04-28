#include "Igniter.h"

//make struct 'Igniter', fill it, return pointer to it    (names of args start with '_' sign)
Igniter* igniter_init(GPIO_TypeDef* _GPIO_PORT_IGNITER, uint16_t _PIN_IGNITER, GPIO_TypeDef* _GPIO_PORT_TEST_CON, uint16_t _PIN_TEST_CON){

	Igniter* I = malloc(sizeof(Igniter));	 //allocate space for struct
	//fill whole struct
	I->GPIO_PORT_IGNITER = _GPIO_PORT_IGNITER;
	I->PIN_IGNITER = _PIN_IGNITER;
	I->GPIO_PORT_TEST_CON = _GPIO_PORT_TEST_CON;
	I->PIN_TEST_CON = _PIN_TEST_CON;
}

bool igniter_is_connected(Igniter* Igniter){
	if(HAL_GPIO_ReadPin(Igniter->GPIO_PORT_TEST_CON, Igniter->PIN_TEST_CON) == 1)
		return 1;
	else
		return 0;
}

bool igniter_FIRE(Igniter* Igniter){
	HAL_GPIO_WritePin(Igniter->GPIO_PORT_IGNITER, Igniter->PIN_IGNITER, 1);
	if(igniter_is_connected(Igniter) == 1)
		return 1;
	else
		return 0;
}


