#ifndef BLUETOOTH_LIB
#define BLUETOOTH_LIB // spelling error

#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "tim.h"
#include "stdlib.h"
#include "usart.h"





typedef struct Bluetooth_module{
	//USART
	UART_HandleTypeDef* huart;

}Bluetooth_module;





////////////////////VARIABLES//////////////////////
char buff [50];
uint8_t timcnt;
uint8_t buffindex;
//////////////////////////////////////////////////



Bluetooth_module* bluetooth_init(UART_HandleTypeDef* _huart);


/////////////////////////FUNCTIONS//////////////////////////////////

bool stringCompare(char array1[], char array2[], uint16_t lght);
void interrupt_USART(UART_HandleTypeDef *_huart);
void interrupt_TIM();
//void doCommand(Bluetooth_module *Module, Motor *Mot, Igniter* igniter);
void doCommand1();
void resolveCommand();
///////////////////////////////////////////////////////////////////
#endif

