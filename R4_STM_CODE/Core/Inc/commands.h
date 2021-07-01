/*
 * commands.h
 *
 *  Created on: 30 cze 2021
 *      Author: KooBeK
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_


#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "tim.h"
#include "stdlib.h"
#include "usart.h"
#include "L298.h"
#include "Bluetooth.h"


void doCommand(Bluetooth_module *Module, Motor *Mot, Motor *Depr, Motor *Quickrel);




#endif /* INC_COMMANDS_H_ */
