#include <Igniter.hh>

//(names of args start with '_' sign)
Igniter::Igniter(GPIO_TypeDef* IgniterGPIOPort_, uint16_t IgniterPin_, GPIO_TypeDef* IgniterConnectionTestGPIOPort_, uint16_t IgniterConnectionTestPin_){
	GPIO_PORT_IGNITER = IgniterGPIOPort_;
	PIN_IGNITER = IgniterPin_;
	GPIO_PORT_TEST_CON = IgniterConnectionTestGPIOPort_;
	PIN_TEST_CON = IgniterConnectionTestPin_;
	tried = false;
}

uint8_t Igniter::GetStatus() const{
	if(tried == true && HAL_GPIO_ReadPin(GPIO_PORT_TEST_CON, PIN_TEST_CON) == GPIO_PIN_SET)
		return 3;
	else if(tried == true && HAL_GPIO_ReadPin(GPIO_PORT_TEST_CON, PIN_TEST_CON) == GPIO_PIN_RESET)
		return 2;
	else if(HAL_GPIO_ReadPin(GPIO_PORT_TEST_CON, PIN_TEST_CON) == GPIO_PIN_SET)
		return 1;
	else
		return 0;
}

void Igniter::FIRE(uint16_t Milisecs){
	HAL_GPIO_WritePin(GPIO_PORT_IGNITER, PIN_IGNITER, GPIO_PIN_SET);
	HAL_Delay(Milisecs);
	HAL_GPIO_WritePin(GPIO_PORT_IGNITER, PIN_IGNITER, GPIO_PIN_RESET);
	tried = true;
}
