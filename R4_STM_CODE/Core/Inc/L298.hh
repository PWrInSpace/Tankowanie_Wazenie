#ifndef L298_LIB
#define L298_LIB

#include "stdbool.h"
#include "gpio.h"
#include "tim.h"
#include "stdlib.h"

enum Status {
	OPEN, CLOSE, IDK
};

class Motor {
private:
	GPIO_TypeDef *GPIO_PORT_IN1;
	uint16_t PIN_IN1;
	//Input 2
	GPIO_TypeDef *GPIO_PORT_IN2;
	uint16_t PIN_IN2;
	//Enable
	TIM_HandleTypeDef *TIM_NR_EN;
	uint16_t TIM_CHANNEL_EN;
	//Limit Switch 1
	GPIO_TypeDef *GPIO_PORT_LS_OPEN;
	uint16_t PIN_LS_OPEN;
	//Limit Switch 2
	GPIO_TypeDef *GPIO_PORT_LS_CLOSE;
	uint16_t PIN_LS_CLOSE;
	Status status;
public:
	Motor(GPIO_TypeDef *_GPIO_PORT_IN1, uint16_t _PIN_IN1,
			GPIO_TypeDef *_GPIO_PORT_IN2, uint16_t _PIN_IN2,
			TIM_HandleTypeDef *_TIM_NR_EN, uint16_t _TIM_CHANNEL_EN,
			GPIO_TypeDef *_GPIO_PORT_LS_OPEN, uint16_t _PIN_LS_OPEN,
			GPIO_TypeDef *_GPIO_PORT_LS_CLOSE, uint16_t _PIN_LS_CLOSE);

	void stop();
	void open(uint8_t secs = 5);
	void close(uint8_t secs = 5);
	void test_open_close();
};

static Motor* Fill(FILL_OPEN_GPIO_Port, FILL_OPEN_Pin, FILL_CLOSE_GPIO_Port,
		FILL_CLOSE_Pin, &htim4, TIM_CHANNEL_3,
		FILL_O_LIMIT_SW_GPIO_Port, FILL_O_LIMIT_SW_Pin,
		FILL_C_LIMIT_SW_GPIO_Port, FILL_C_LIMIT_SW_Pin);

static  Motor* Depr(DEPR_OPEN_GPIO_Port, DEPR_OPEN_Pin, DEPR_CLOSE_GPIO_Port,
		DEPR_CLOSE_Pin, &htim4, TIM_CHANNEL_3,
		DEPR_O_LIMIT_SW_GPIO_Port, DEPR_O_LIMIT_SW_Pin,
		DEPR_C_LIMIT_SW_GPIO_Port, DEPR_C_LIMIT_SW_Pin);

//static Motor Quickrel(QD_D1_GPIO_Port, QD_OPEN_Pin, _GPIO_PORT_IN2, _PIN_IN2, _TIM_NR_EN, _TIM_CHANNEL_EN, _GPIO_PORT_LS_OPEN, _PIN_LS_OPEN, _GPIO_PORT_LS_CLOSE, _PIN_LS_CLOSE)

#endif /* L298_LIB */

