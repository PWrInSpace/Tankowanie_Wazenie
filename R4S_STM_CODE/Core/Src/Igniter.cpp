#include <Igniter.hh>

//(names of args start with '_' sign)
Igniter::Igniter(GPIO_TypeDef* _GPIO_PORT_IGNITER, uint16_t _PIN_IGNITER, GPIO_TypeDef* _IGN_GPIO_PORT_TEST_CON, uint16_t _IGN_PIN_TEST_CON){
	GPIO_PORT_IGNITER = _GPIO_PORT_IGNITER;
	PIN_IGNITER = _PIN_IGNITER;
	GPIO_PORT_TEST_CON = _IGN_GPIO_PORT_TEST_CON;
	PIN_TEST_CON = _IGN_PIN_TEST_CON;
}

bool Igniter::is_connected() const{
	if(HAL_GPIO_ReadPin(GPIO_PORT_TEST_CON, PIN_TEST_CON) == GPIO_PIN_SET)
		return 1;
	else
		return 0;
}

void Igniter::FIRE(uint8_t milisecs){
	HAL_GPIO_WritePin(GPIO_PORT_IGNITER, PIN_IGNITER, GPIO_PIN_SET);
	HAL_Delay(milisecs);
	HAL_GPIO_WritePin(GPIO_PORT_IGNITER, PIN_IGNITER, GPIO_PIN_RESET);
}


