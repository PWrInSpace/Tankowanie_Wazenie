#ifndef L298_LIB
#define L298_LIB

#include "stdbool.h"
#include "gpio.h"
#include "tim.h"
#include "stdlib.h"

enum Status {CLOSE = 0, OPEN = 1, IDK = 2};

class Motor{
private:
	GPIO_TypeDef* GPIO_PORT_IN1;
	uint16_t PIN_IN1;
	//Input 2
	GPIO_TypeDef* GPIO_PORT_IN2;
	uint16_t PIN_IN2;
	//Enable
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
	Motor(GPIO_TypeDef* _GPIO_PORT_IN1, uint16_t _PIN_IN1,
			GPIO_TypeDef* _GPIO_PORT_IN2, uint16_t _PIN_IN2,
			TIM_HandleTypeDef* _TIM_NR_EN, uint16_t _TIM_CHANNEL_EN,
			GPIO_TypeDef* _GPIO_PORT_LS_OPEN = nullptr, uint16_t _PIN_LS_OPEN = 0,
			GPIO_TypeDef* _GPIO_PORT_LS_CLOSE = nullptr, uint16_t _PIN_LS_CLOSE = 0);
	Status getStatus();
	void stop();
	void open(uint8_t secs = 5);
	void close(uint8_t secs = 5);
	void test_open_close();
};

#endif /* L298_LIB */
