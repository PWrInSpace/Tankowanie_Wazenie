#ifndef SERWO_LIB
#define SERWO_LIB

#include "stdbool.h"
#include "gpio.h"
#include "tim.h"
#include "stdlib.h"

enum Status {OPEN, CLOSE, IDK};

class Serwo{
private:
	//Enable (PWM)
	TIM_HandleTypeDef* TIM_NR_EN;
	uint16_t TIM_CHANNEL_EN;
	//Limit Switch 1
	GPIO_TypeDef* GPIO_PORT_LS_OPEN;
	uint16_t PIN_LS_OPEN;
	//Limit Switch 2
	GPIO_TypeDef* GPIO_PORT_LS_CLOSE;
	uint16_t PIN_LS_CLOSE;
	Status status;
public:
	Serwo(	TIM_HandleTypeDef* _TIM_NR_EN, uint16_t _TIM_CHANNEL_EN,
			GPIO_TypeDef* _GPIO_PORT_LS_OPEN, uint16_t _PIN_LS_OPEN,
			GPIO_TypeDef* _GPIO_PORT_LS_CLOSE, uint16_t _PIN_LS_CLOSE);
	//void stop();
	void setAngle(uint8_t angle);
	void open(uint8_t secs = 5);
	void close(uint8_t secs = 5);
	void test_open_close();
};
	uint16_t mapAngleToPWM(uint8_t angle, uint8_t PWMMin = 0, uint16_t PWMMAx = 1000,
						   uint8_t angleMin = 0, uint8_t angleMax = 180);

#endif /* SERWO_LIB */
