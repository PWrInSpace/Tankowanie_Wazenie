#include "Funkcje.h"

//make struct 'Motor', fill it, return pointer to it    (names for args start with '_' sign)
Motor* Motor_Init(GPIO_TypeDef* _GPIO_PORT_IN1, uint16_t _PIN_IN1, GPIO_TypeDef* _GPIO_PORT_IN2, uint16_t _PIN_IN2, TIM_HandleTypeDef _TIM_NR_EN, uint16_t _TIM_CHANNEL_EN,
		GPIO_TypeDef* _GPIO_PORT_LS_OPEN, uint16_t _PIN_LS_OPEN, GPIO_TypeDef* _GPIO_PORT_LS_CLOSED, uint16_t _PIN_LS_CLOSED ){
	 //allocate space for struct
	Motor* M = malloc(sizeof(Motor));
	 //fill struct
	M->GPIO_PORT_IN1 = _GPIO_PORT_IN1;
	M->PIN_IN1 = _PIN_IN1;
	M->GPIO_PORT_IN2 = _GPIO_PORT_IN2;
	M->PIN_IN2 = _PIN_IN2;
	M->TIM_NR_EN = _TIM_NR_EN;
	M->TIM_CHANNEL_EN = _TIM_CHANNEL_EN;
	M->GPIO_PORT_LS_OPEN = _GPIO_PORT_LS_OPEN;
	M->PIN_LS_OPEN = _PIN_LS_OPEN;
	M->GPIO_PORT_LS_CLOSED = _GPIO_PORT_LS_CLOSED;
	M->PIN_LS_CLOSED = _PIN_LS_CLOSED;
	return M;
};

void motor_stop(Motor *Mot){
	__HAL_TIM_SET_COMPARE(&(Mot->TIM_NR_EN), Mot->TIM_CHANNEL_EN, 0);
	HAL_GPIO_WritePin(Mot->GPIO_PORT_IN1, Mot->PIN_IN1, 0);
	HAL_GPIO_WritePin(Mot->GPIO_PORT_IN2, Mot->PIN_IN2, 0);
}

void motor_opening(Motor *Mot){
	__HAL_TIM_SET_COMPARE(&(Mot->TIM_NR_EN), Mot->TIM_CHANNEL_EN, 1000);
	HAL_GPIO_WritePin(Mot->GPIO_PORT_IN1, Mot->PIN_IN1, 1);
	HAL_GPIO_WritePin(Mot->GPIO_PORT_IN2, Mot->PIN_IN2, 0);
}

void motor_closing(Motor *Mot){
	__HAL_TIM_SET_COMPARE(&(Mot->TIM_NR_EN), Mot->TIM_CHANNEL_EN, 1000);
	HAL_GPIO_WritePin(Mot->GPIO_PORT_IN1, Mot->PIN_IN1, 0);
	HAL_GPIO_WritePin(Mot->GPIO_PORT_IN2, Mot->PIN_IN2, 1);
}


void motor_initial(Motor *Mot){
	bool dir = false;
	uint8_t i=0;

	while(i<2){
		do{
			Mot->State_of_limit_switch_open = HAL_GPIO_ReadPin(Mot->GPIO_PORT_LS_OPEN, Mot->PIN_LS_OPEN);
			Mot->State_of_limit_switch_closed = HAL_GPIO_ReadPin(Mot->GPIO_PORT_LS_CLOSED, Mot->PIN_LS_CLOSED);
			motor_closing(Mot);
		}while(Mot->State_of_limit_switch_open == 0 && Mot->State_of_limit_switch_closed == 0);

		if (Mot->State_of_limit_switch_open == 1){
			dir = false;
			}
		if (Mot->State_of_limit_switch_closed == 1){
			 dir = true;
			 }
		if (dir){
			do{
				motor_opening(Mot);
				Mot->State_of_limit_switch_open  = HAL_GPIO_ReadPin(Mot->GPIO_PORT_LS_OPEN, Mot->PIN_LS_OPEN);
			}while(Mot->State_of_limit_switch_open != 1);
			i++;
		}
		else if (!dir){
			do{
				motor_closing(Mot);
				Mot->State_of_limit_switch_closed = HAL_GPIO_ReadPin(Mot->GPIO_PORT_LS_CLOSED, Mot->PIN_LS_CLOSED);
			}while(Mot->State_of_limit_switch_closed != 1);
			i++;
		}
	}

	Mot->State_of_limit_switch_open  = HAL_GPIO_ReadPin(Mot->GPIO_PORT_LS_OPEN, Mot->PIN_LS_OPEN);
	if(Mot->State_of_limit_switch_open == 1){
		do{
			motor_closing(Mot);
			Mot->State_of_limit_switch_closed = HAL_GPIO_ReadPin(Mot->GPIO_PORT_LS_CLOSED, Mot->PIN_LS_CLOSED);
		}while(Mot->State_of_limit_switch_closed != 1);
	}
	motor_stop(Mot);
}
