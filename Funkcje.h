#ifndef INC_FUNKCJE_H_
#define INC_FUNKCJE_H_

#include "stdbool.h"
#include "gpio.h"
#include "tim.h"
#include "stdlib.h"

typedef struct {
	//IN1
	GPIO_TypeDef* GPIO_PORT_IN1;
	uint16_t PIN_IN1;
	//IN2
	GPIO_TypeDef* GPIO_PORT_IN2;
	uint16_t PIN_IN2;
	//EN
	TIM_HandleTypeDef TIM_NR_EN;
	uint16_t TIM_CHANNEL_EN;
}Motor;



//void SystemClock_Config(void);

int state_of_limit_switch_open;
int state_of_limit_switch_close;

void motor_stop();
void motor_opening();
void motor_closing();
void motor_initial();

#endif /* INC_FUNKCJE_H_ */
