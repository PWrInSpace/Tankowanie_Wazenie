#ifndef BLUETOTTH_LIB
#define BLUETOTTH_LIB

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "stdlib.h"

void resolveCommand(UART_HandleTypeDef* huart1);
bool stringCompare(char array1[], char array2[], uint16_t lght);

#ifdef __cplusplus
}
#endif

#endif
