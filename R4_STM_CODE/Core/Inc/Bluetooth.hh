#ifndef BLUETOOTH_LIB
#define BLUETOOTH_LIB // spelling error

#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "tim.h"
#include "stdlib.h"
#include "usart.h"

#define MAX_BUFF 50

class Bluetooth {
	//USART
public:
	/////////////////////////VARS//////////////////////////////////
	UART_HandleTypeDef *huart;





	/////////////////////////FUNCTIONS//////////////////////////////////
	Bluetooth(UART_HandleTypeDef *_huart);

};

static	char buff [MAX_BUFF];
static	uint8_t timcnt;
static	uint8_t buffindex;

bool stringCompare(char array1[], char array2[], uint16_t lght);
void interrupt_USART(UART_HandleTypeDef *huart);
void interrupt_TIM();
void resolveCommand();
void doCommand1();

///////////////////////////////////////////////////////////////////
#endif /* BLUETOOTH_LIB */

