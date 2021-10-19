#ifndef L298_LIB
#define L298_LIB

#include "Valve.hh"
#include "stdbool.h"
#include "gpio.h"
#include "tim.h"
#include "stdlib.h"
#include <string>

class Motor: public ValveInterface{
private:
	GPIO_TypeDef* Input1GPIOPort;
	uint16_t Input1Pin;
	GPIO_TypeDef* Input2GPIOPort;
	uint16_t Input2Pin;
	TIM_HandleTypeDef* PWMTimerNumber;
	uint16_t PWMTimerChannel;
	GPIO_TypeDef* LimitSwitchOpenGPIOPort;
	uint16_t LimitSwitchOpenPin;
	GPIO_TypeDef* LimitSwitchCloseGPIOPort;
	uint16_t LimitSwitchClosePin;
public:
	Motor(GPIO_TypeDef* Input1GPIOPort, uint16_t Input1Pin,
			GPIO_TypeDef* Input2GPIOPort, uint16_t Input2Pin,
			TIM_HandleTypeDef* PWMTimerNumber, uint16_t PWMTimerChannel, 	//TIMER not GPIOPort (&htim3 = OK ; FILL_EN_PORT = NOT OK)
			GPIO_TypeDef* LimitSwitchOpenGPIOPort = nullptr, uint16_t LimitSwitchOpenPin = 0,
			GPIO_TypeDef* LimitSwitchCloseGPIOPort = nullptr, uint16_t LimitSwitchClosePin = 0);
	void Stop() override;
	void Open(uint32_t Milisecs = 3000) override;
	void Close(uint32_t Milisecs = 3000) override;
	void ValveCommandHandler(char Command, uint32_t Milisecs = 3000);
};

#endif /* L298_LIB */
