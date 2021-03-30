#ifndef INC_L298_H_
#define INC_L298_H_

#include "stdbool.h"
#include "gpio.h"
#include "tim.h"
#include "stdlib.h"

typedef struct {
	//Input 1
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
	GPIO_TypeDef* GPIO_PORT_LS_CLOSED;
	uint16_t PIN_LS_CLOSED;
	bool State_of_limit_switch_open;
	bool State_of_limit_switch_closed;
}Motor;

//?? void SystemClock_Config(void);

Motor* Motor_Init(GPIO_TypeDef* _GPIO_PORT_IN1, uint16_t _PIN_IN1, GPIO_TypeDef* _GPIO_PORT_IN2, uint16_t _PIN_IN2,
		TIM_HandleTypeDef* _TIM_NR_EN, uint16_t _TIM_CHANNEL_EN, GPIO_TypeDef* _GPIO_PORT_LS_OPEN, uint16_t _PIN_LS_OPEN,
		GPIO_TypeDef* _GPIO_PORT_LS_CLOSED, uint16_t _PIN_LS_CLOSED );


int state_of_limit_switch_open;
int state_of_limit_switch_close;

void motor_stop(Motor *Mot);
void motor_opening(Motor *Mot);
void motor_closing(Motor *Mot);
void motor_initial(Motor *Mot);

#endif /* INC_L298_H_ */
