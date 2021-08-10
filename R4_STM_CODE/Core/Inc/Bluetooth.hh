#ifndef BLUETOOTH_LIB
#define BLUETOOTH_LIB

#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "tim.h"
#include "stdlib.h"
#include "usart.h"
#include "main.hh"
#include <Commands.hh>

#define MAX_BUFF 50

class Bluetooth {
	//USART
private:
	char buff[MAX_BUFF];
		uint8_t timcnt;
		uint8_t buffindex;
public:
	/////////////////////////VARS//////////////////////////////////
	UART_HandleTypeDef *huart;
	Commands commands;
	/////////////////////////FUNCTIONS//////////////////////////////////
	Bluetooth(UART_HandleTypeDef *_huart, Commands _commands);
	void setFunc(void(*func)(void));
	void resolveCommand();
	void interrupt_USART();
	void interrupt_TIM();




};



//bool stringCompare(const char array1[], const char array2[], uint16_t lght);
Commands getCommands();


void doCommand1();

///////////////////////////////////////////////////////////////////
/* BLUETOOTH_LIB */
#endif
