#ifndef BLUETOOTH_LIB
#define BLUETOOTH_LIB // spelling error

#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "stdlib.h"
#include <L298.h>




////////////////////VARIABLES//////////////////////
char buff [50];
uint8_t timcnt;
uint8_t buffindex;
//////////////////////////////////////////////////






/////////////////////////FUNCTIONS//////////////////////////////////
void resolveCommand(UART_HandleTypeDef* huart1, Motor* Mot);
bool stringCompare(char array1[], char array2[], uint16_t lght);
///////////////////////////////////////////////////////////////////
#endif
