#include <MAX14870.hh>

Motor::Motor(	GPIO_TypeDef* DirGPIOPort_,
				uint16_t DirPin_,
				TIM_HandleTypeDef* PWMTimerNumber_,
				uint16_t PWMTimerChannel_)
:	ValveInterface(),
	DirGPIOPort(DirGPIOPort_),
	DirPin(DirPin_),
	PWMTimerNumber(PWMTimerNumber_),
	PWMTimerChannel(PWMTimerChannel_)
{
	Stop();
}
void Motor::Stop(){
	__HAL_TIM_SET_COMPARE(PWMTimerNumber, PWMTimerChannel, 0);
}
void Motor::Open(){
	HAL_GPIO_WritePin(DirGPIOPort, DirPin, GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(PWMTimerNumber, PWMTimerChannel, 999);
}

void Motor::Close(){
	HAL_GPIO_WritePin(DirGPIOPort, DirPin, GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(PWMTimerNumber, PWMTimerChannel, 999);
}
