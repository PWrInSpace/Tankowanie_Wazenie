#ifndef BLUETOOTH_LIB
#define BLUETOOTH_LIB

#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "tim.h"
#include "stdlib.h"
#include "usart.h"
#include "main.hh"
#include <Rocket.hh>

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
	Rocket rocket;
	/////////////////////////FUNCTIONS//////////////////////////////////
	Bluetooth(UART_HandleTypeDef *_huart, Rocket _rocket);
	void setFunc(void(*func)(void));
	void resolveCommand();
	void interrupt_USART();
	void interrupt_TIM();




};



//bool stringCompare(const char array1[], const char array2[], uint16_t lght);
//Commands getCommands();


void doCommand1();

///////////////////////////////////////////////////////////////////
/* BLUETOOTH_LIB */
#endif
