#include <Serwo.hh>

//make struct 'Serwo', fill it, return pointer to it    (names for args start with '_' sign)
Serwo::Serwo(TIM_HandleTypeDef* _TIM_NR_EN, uint16_t _TIM_CHANNEL_EN,
			GPIO_TypeDef* _GPIO_PORT_LS_OPEN, uint16_t _PIN_LS_OPEN,
			GPIO_TypeDef* _GPIO_PORT_LS_CLOSE, uint16_t _PIN_LS_CLOSE)
{
	TIM_NR_EN = _TIM_NR_EN;
	TIM_CHANNEL_EN = _TIM_CHANNEL_EN;
	GPIO_PORT_LS_OPEN = _GPIO_PORT_LS_OPEN;
	PIN_LS_OPEN = _PIN_LS_OPEN;
	GPIO_PORT_LS_CLOSE = _GPIO_PORT_LS_CLOSE;
	PIN_LS_CLOSE = _PIN_LS_CLOSE;
	status = Status::IDK;
	HAL_TIM_PWM_Start(TIM_NR_EN, TIM_CHANNEL_EN);
}

uint16_t mapAngleToPWM(uint8_t angle, uint8_t PWMMin, uint16_t PWMMAx,
						   uint8_t angleMin, uint8_t angleMax){
	return (angle - angleMin) * (PWMMAx - PWMMin) / (angleMax - angleMin) + PWMMin;
}

void Serwo::setAngle(uint8_t angle){
	__HAL_TIM_SET_COMPARE(TIM_NR_EN, TIM_CHANNEL_EN, mapAngleToPWM(angle));
}

void Serwo::open(uint8_t secs){
	setAngle(180);
	status = Status::IDK;
	for(uint8_t steps =  0; steps < secs * 100 ; ++steps ){
		if(GPIO_PORT_LS_CLOSE != nullptr && GPIO_PORT_LS_OPEN != nullptr){ //if has limit switch
			if(HAL_GPIO_ReadPin(GPIO_PORT_LS_OPEN, PIN_LS_OPEN) == GPIO_PIN_RESET){
				status = Status::OPEN;
				break;
			}
		}
		HAL_Delay(10);
	}
}

void Serwo::close(uint8_t secs){
	setAngle(0);
	status = Status::IDK;
	for(uint8_t steps =  0; steps < secs * 100 ; ++steps ){
		if(GPIO_PORT_LS_CLOSE != nullptr && GPIO_PORT_LS_OPEN != nullptr){ //if has limit switch
			if(HAL_GPIO_ReadPin(GPIO_PORT_LS_OPEN, PIN_LS_OPEN) == GPIO_PIN_RESET){
				status = Status::OPEN;
				break;
			}
		}
		HAL_Delay(10);
	}
}

void Serwo::test_open_close(){
	open();
	HAL_Delay(100);
	close();
	HAL_Delay(100);
	open();
	HAL_Delay(100);
	close();
}
