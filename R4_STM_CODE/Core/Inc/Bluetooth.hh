#ifndef BLUETOOTH_LIB
#define BLUETOOTH_LIB

#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "tim.h"
#include "stdlib.h"
#include "usart.h"
#include <main.hh>
#include <Commands.hh>

#define MAX_BUFF 50

class Bluetooth {
	//USART
private:
	static Commands* commands = new Commands();
public:
	/////////////////////////VARS//////////////////////////////////
	UART_HandleTypeDef *huart;

	/////////////////////////FUNCTIONS//////////////////////////////////
	Bluetooth(UART_HandleTypeDef *_huart);
	void setFunc(void(*func)(void));
	static void resolveCommand();

};

static char buff[MAX_BUFF];
static uint8_t timcnt;
static uint8_t buffindex;

//bool stringCompare(const char array1[], const char array2[], uint16_t lght);
Commands* getCommands();
void interrupt_USART(UART_HandleTypeDef *huart);
void interrupt_TIM();

void doCommand1();

///////////////////////////////////////////////////////////////////
/* BLUETOOTH_LIB */
#endif
