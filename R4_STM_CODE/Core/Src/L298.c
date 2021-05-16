#include <L298.h>

//make struct 'Motor', fill it, return pointer to it    (names for args start with '_' sign)
Motor* motor_init(GPIO_TypeDef* _GPIO_PORT_IN1, uint16_t _PIN_IN1, GPIO_TypeDef* _GPIO_PORT_IN2, uint16_t _PIN_IN2,
		TIM_HandleTypeDef* _TIM_NR_EN, uint16_t _TIM_CHANNEL_EN, GPIO_TypeDef* _GPIO_PORT_LS_OPEN, uint16_t _PIN_LS_OPEN,
		GPIO_TypeDef* _GPIO_PORT_LS_CLOSE, uint16_t _PIN_LS_CLOSE){
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
	M->GPIO_PORT_LS_CLOSE = _GPIO_PORT_LS_CLOSE;
	M->PIN_LS_CLOSE = _PIN_LS_CLOSE;
	return M;
};

void motor_stop(Motor *Mot){
	__HAL_TIM_SET_COMPARE(Mot->TIM_NR_EN, Mot->TIM_CHANNEL_EN, 0);
	HAL_GPIO_WritePin(Mot->GPIO_PORT_IN1, Mot->PIN_IN1, 0);
	HAL_GPIO_WritePin(Mot->GPIO_PORT_IN2, Mot->PIN_IN2, 0);
}

void motor_opening(Motor *Mot){
	__HAL_TIM_SET_COMPARE(Mot->TIM_NR_EN, Mot->TIM_CHANNEL_EN, 1000);
	do{
		HAL_GPIO_WritePin(Mot->GPIO_PORT_IN1, Mot->PIN_IN1, 1);
		HAL_GPIO_WritePin(Mot->GPIO_PORT_IN2, Mot->PIN_IN2, 0);
	}while(HAL_GPIO_ReadPin(Mot->GPIO_PORT_LS_OPEN, Mot->PIN_LS_OPEN) == 0);
	motor_stop(Mot);
}

void motor_closing(Motor *Mot){
	__HAL_TIM_SET_COMPARE(Mot->TIM_NR_EN, Mot->TIM_CHANNEL_EN, 1000);
	do{
		HAL_GPIO_WritePin(Mot->GPIO_PORT_IN1, Mot->PIN_IN1, 0);
		HAL_GPIO_WritePin(Mot->GPIO_PORT_IN2, Mot->PIN_IN2, 1);
	}while(HAL_GPIO_ReadPin(Mot->GPIO_PORT_LS_CLOSE, Mot->PIN_LS_CLOSE) == 0);
	motor_stop(Mot);
}

void motor_initial(Motor *Mot){
	motor_opening(Mot);
	motor_closing(Mot);
	motor_opening(Mot);
	motor_closing(Mot);
	motor_stop(Mot);
}
