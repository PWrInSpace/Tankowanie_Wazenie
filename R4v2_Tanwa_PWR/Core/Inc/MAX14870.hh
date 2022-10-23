#ifndef INC_MAX14870_HH_
#define INC_MAX14870_HH_

#include <Valve.hh>
#include "stm32f1xx_hal.h"

class Motor: public ValveInterface{
	GPIO_TypeDef* DirGPIOPort;
	uint16_t DirPin;
	TIM_HandleTypeDef* PWMTimerNumber;
	uint16_t PWMTimerChannel;
public:
	Motor(	GPIO_TypeDef* DirGPIOPort_,
			uint16_t DirPin_,
			TIM_HandleTypeDef* PWMTimerNumber_,
			uint16_t PWMTimerChannel_);
	void Stop() override;
	void Open() override;
	void Close() override;
};

#endif /* INC_MAX14870_HH_ */
