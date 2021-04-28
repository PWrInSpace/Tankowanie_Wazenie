#ifndef BLUETOTTH_LIB
#define BLUETOTTH_LIB

#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "stdlib.h"

#include <L298.h>

void resolveCommand(UART_HandleTypeDef* huart1, Motor* Mot);
bool stringCompare(char array1[], char array2[], uint16_t lght);

#endif
