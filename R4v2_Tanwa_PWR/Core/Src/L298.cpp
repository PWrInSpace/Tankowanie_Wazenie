#include <L298.hh>

OldMotor::OldMotor(GPIO_TypeDef* Input1GPIOPort_, uint16_t Input1Pin_,
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

void OldMotor::Stop(){
	__HAL_TIM_SET_COMPARE(PWMTimerNumber, PWMTimerChannel, 0);
	HAL_GPIO_WritePin(Input1GPIOPort, Input1Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Input2GPIOPort, Input2Pin, GPIO_PIN_RESET);
}

void OldMotor::Open(){
	HAL_GPIO_WritePin(Input1GPIOPort, Input1Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Input2GPIOPort, Input2Pin, GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(PWMTimerNumber, PWMTimerChannel, 999);
	State = ValveState::ValveStateAttemptToOpen;
}

void OldMotor::Close(){
	HAL_GPIO_WritePin(Input1GPIOPort, Input1Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Input2GPIOPort, Input2Pin, GPIO_PIN_SET);
	__HAL_TIM_SET_COMPARE(PWMTimerNumber, PWMTimerChannel, 999);
	State = ValveState::ValveStateAttemptToClose;
}

void OldMotor::ValveCommandHandler(char Command, uint32_t Input){
	if(Command == 'O')
		Open();
	else if(Command == 'C')
		Close();
	else if(Command == 'S'){
		if(Input == 1 && State == ValveStateAttemptToOpen)
			Stop();
		else if(Input == 0 && State == ValveStateAttemptToClose)
			Stop();
	}
}
