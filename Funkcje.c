#include "Funkcje.h"

//make struct 'Motor', fill it, return pointer to it    (names for args start with '_' sign)
Motor* Motor_Init(GPIO_TypeDef* _GPIO_PORT_IN1, uint16_t _PIN_IN1, GPIO_TypeDef* _GPIO_PORT_IN2, uint16_t _PIN_IN2, TIM_HandleTypeDef _TIM_NR_EN, uint16_t _TIM_CHANNEL_EN){
	//allocate the space for struct
	Motor* M = malloc(sizeof(Motor));
	M->GPIO_PORT_IN1 = malloc(sizeof(GPIO_TypeDef));
	M->GPIO_PORT_IN2 = malloc(sizeof(GPIO_TypeDef));
	//fill struct
	M->GPIO_PORT_IN1 = _GPIO_PORT_IN1;
	M->PIN_IN1 = _PIN_IN1;
	M->GPIO_PORT_IN2 = _GPIO_PORT_IN2;
	M->PIN_IN2 = _PIN_IN2;
	M->TIM_NR_EN = _TIM_NR_EN;
	M->TIM_CHANNEL_EN = _TIM_CHANNEL_EN;
	return M;
};
/*
void motor_stop(){

	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
	HAL_GPIO_WritePin(LEDIN1_GPIO_Port, LEDIN1_Pin, 0);
	HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, 0);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState)
}


void motor_opening(){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 1000);
	HAL_GPIO_WritePin(LEDIN1_GPIO_Port, LEDIN1_Pin, 1);
	HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, 0);
}

void motor_closing(){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 1000);
	HAL_GPIO_WritePin(LEDIN1_GPIO_Port, LEDIN1_Pin, 0);
	HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, 1);
}


void motor_initial(){
	bool dir=false;
	int i=0;

while(i<2){


	do{
		state_of_limit_switch_open  = HAL_GPIO_ReadPin(LIMIT_SWITCH_OPEN_GPIO_Port, LIMIT_SWITCH_OPEN_Pin);
		state_of_limit_switch_close = HAL_GPIO_ReadPin(LIMIT_SWITCH_CLOSE_GPIO_Port, LIMIT_SWITCH_CLOSE_Pin);
		motor_closing();
	}while(state_of_limit_switch_open == 0 && state_of_limit_switch_close == 0);

	if (state_of_limit_switch_open == 1) {
		dir = false;
		}

	if (state_of_limit_switch_close == 1) {
		 dir = true;
		 }


	if (dir) {
		do{
			motor_opening();
			state_of_limit_switch_open  = HAL_GPIO_ReadPin(LIMIT_SWITCH_OPEN_GPIO_Port, LIMIT_SWITCH_OPEN_Pin);
		}while(state_of_limit_switch_open != 1);
		i++;
	 }

	else if (!dir) {
		do{
			motor_closing();
			state_of_limit_switch_close = HAL_GPIO_ReadPin(LIMIT_SWITCH_CLOSE_GPIO_Port, LIMIT_SWITCH_CLOSE_Pin);
		}while(state_of_limit_switch_close != 1);
		i++;
	}
	 }

state_of_limit_switch_open  = HAL_GPIO_ReadPin(LIMIT_SWITCH_OPEN_GPIO_Port, LIMIT_SWITCH_OPEN_Pin);
	if(state_of_limit_switch_open == 1){
		do{
			motor_closing();
			state_of_limit_switch_close = HAL_GPIO_ReadPin(LIMIT_SWITCH_CLOSE_GPIO_Port, LIMIT_SWITCH_CLOSE_Pin);
		}while(state_of_limit_switch_close != 1);
	}
	motor_stop();
}
/*
