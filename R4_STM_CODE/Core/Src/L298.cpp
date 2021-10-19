#include <L298.hh>

Motor::Motor(GPIO_TypeDef* Input1GPIOPort_, uint16_t Input1Pin_,
		GPIO_TypeDef* Input2GPIOPort_, uint16_t Input2Pin_,
		TIM_HandleTypeDef* PWMTimerNumber_, uint16_t PWMTimerChannel_, 	//TIMER not GPIOPort (&htim3 = OK ; FILL_EN_PORT = NOT OK)
		GPIO_TypeDef* LimitSwitchOpenGPIOPort_, uint16_t LimitSwitchOpenPin_,
		GPIO_TypeDef* LimitSwitchCloseGPIOPort_, uint16_t LimitSwitchClosePin_)
  : ValveInterface(),
	Input1GPIOPort(Input1GPIOPort_), Input1Pin(Input1Pin_),
	Input2GPIOPort(Input2GPIOPort_), Input2Pin (Input2Pin_),
	PWMTimerNumber(PWMTimerNumber_), PWMTimerChannel(PWMTimerChannel_),
	LimitSwitchOpenGPIOPort(LimitSwitchOpenGPIOPort_), LimitSwitchOpenPin(LimitSwitchOpenPin_),
	LimitSwitchCloseGPIOPort(LimitSwitchCloseGPIOPort_), LimitSwitchClosePin(LimitSwitchClosePin_)

{
	HAL_TIM_PWM_Start(PWMTimerNumber_, PWMTimerChannel_);
	Stop();
}

void Motor::Stop(){
	__HAL_TIM_SET_COMPARE(PWMTimerNumber, PWMTimerChannel, 0);
	HAL_GPIO_WritePin(Input1GPIOPort, Input1Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Input2GPIOPort, Input2Pin, GPIO_PIN_RESET);
}

void Motor::Open(uint32_t Milisecs){
	HAL_GPIO_WritePin(Input1GPIOPort, Input1Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Input2GPIOPort, Input2Pin, GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(PWMTimerNumber, PWMTimerChannel, 999);
	State = ValveState::ValveStateAttemptToOpen;
	if(LimitSwitchOpenGPIOPort != nullptr){ //if has limit switch
		for(uint32_t steps =  0; steps < Milisecs / 10 ; ++steps){
			if(HAL_GPIO_ReadPin(LimitSwitchOpenGPIOPort, LimitSwitchOpenPin) == GPIO_PIN_RESET){
				State = ValveState::ValveStateOpen;
				break;
			}
		HAL_Delay(10);
		}
	}
	else
		HAL_Delay(Milisecs);
	Stop();
}

void Motor::Close(uint32_t Milisecs){
	HAL_GPIO_WritePin(Input1GPIOPort, Input1Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Input2GPIOPort, Input2Pin, GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(PWMTimerNumber, PWMTimerChannel, 999);
	State = ValveState::ValveStateAttemptToClose;
	if(LimitSwitchCloseGPIOPort != nullptr){ //if has limit switch
		for(uint32_t steps =  0; steps < Milisecs/10; ++steps){
			if(HAL_GPIO_ReadPin(LimitSwitchCloseGPIOPort, LimitSwitchClosePin) == GPIO_PIN_RESET){
				State = ValveState::ValveStateClose;
				break;
			}
			HAL_Delay(10);
		}
	}
	else
		HAL_Delay(Milisecs);
	Stop();
}

void Motor::ValveCommandHandler(char Command, uint32_t Milisecs){
	if(Command == 'O')
		Open(Milisecs);
	else if(Command == 'Z')
		Close(Milisecs);
}
