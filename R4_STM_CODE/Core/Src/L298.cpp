#include <L298.hh>

//make struct 'Motor', fill it, return pointer to it    (names for args start with '_' sign)
Motor::Motor(GPIO_TypeDef* _GPIO_PORT_IN1, uint16_t _PIN_IN1, GPIO_TypeDef* _GPIO_PORT_IN2, uint16_t _PIN_IN2,
		TIM_HandleTypeDef* _TIM_NR_EN, uint16_t _TIM_CHANNEL_EN, GPIO_TypeDef* _GPIO_PORT_LS_OPEN, uint16_t _PIN_LS_OPEN,
		GPIO_TypeDef* _GPIO_PORT_LS_CLOSE, uint16_t _PIN_LS_CLOSE){

	GPIO_PORT_IN1 = _GPIO_PORT_IN1;
	PIN_IN1 = _PIN_IN1;
	GPIO_PORT_IN2 = _GPIO_PORT_IN2;
	PIN_IN2 = _PIN_IN2;
	TIM_NR_EN = _TIM_NR_EN;
	TIM_CHANNEL_EN = _TIM_CHANNEL_EN;
	GPIO_PORT_LS_OPEN = _GPIO_PORT_LS_OPEN;
	PIN_LS_OPEN = _PIN_LS_OPEN;
	GPIO_PORT_LS_CLOSE = _GPIO_PORT_LS_CLOSE;
	PIN_LS_CLOSE = _PIN_LS_CLOSE;
	status = Status::IDK;
}


void Motor::stop(){
	__HAL_TIM_SET_COMPARE(TIM_NR_EN, TIM_CHANNEL_EN, 0);
	HAL_GPIO_WritePin(GPIO_PORT_IN1, PIN_IN1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO_PORT_IN2, PIN_IN2, GPIO_PIN_RESET);
}

void Motor::open(){
		HAL_GPIO_WritePin(GPIO_PORT_IN1, PIN_IN1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIO_PORT_IN2, PIN_IN2, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(TIM_NR_EN, TIM_CHANNEL_EN, 1000);
		status = Status::IDK;
	if(GPIO_PORT_LS_CLOSE != nullptr && GPIO_PORT_LS_OPEN != nullptr){
		while(HAL_GPIO_ReadPin(GPIO_PORT_LS_OPEN, PIN_LS_OPEN) == GPIO_PIN_RESET);
		stop();
		status = Status::OPEN;
	}
	else{
		HAL_Delay(1000);
		stop();
	}
}

void Motor::close(){
	HAL_GPIO_WritePin(GPIO_PORT_IN1, PIN_IN1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO_PORT_IN2, PIN_IN2, GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(TIM_NR_EN, TIM_CHANNEL_EN, 1000);
	status = Status::IDK;
	if(GPIO_PORT_LS_CLOSE != nullptr && GPIO_PORT_LS_OPEN != nullptr){
		while(HAL_GPIO_ReadPin(GPIO_PORT_LS_CLOSE, PIN_LS_CLOSE) == GPIO_PIN_RESET);
		stop();
		status = Status::CLOSE;
	}
	else{
		HAL_Delay(1000);
		stop();
	}
}

void Motor::test_open_close(){
	open();
	close();
	open();
	close();
	stop();
}
