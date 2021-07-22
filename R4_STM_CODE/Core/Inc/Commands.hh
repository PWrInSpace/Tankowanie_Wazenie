/*
 * commands.h
 *
 *  Created on: 30 cze 2021
 *      Author: KooBeK
 */

#ifndef INC_COMMANDS_HH_
#define INC_COMMANDS_HH_

#include "stdbool.h"
#include "gpio.h"
#include "string.h"
#include "tim.h"
#include "stdlib.h"
#include "usart.h"
#include "L298.hh"
#include <main.hh>





class Commands {
private:
	UART_HandleTypeDef *huart = &huart3;
	//Motor quickrel;
	Motor*fill = &Fill;
	Motor*	depr = &Depr;

public:


	Motor* getFill();
	Motor* getDepr();
	Motor* getQuickRel();
	void doCommand(char buff[]);
	static void doCommand_noacc(char buff[]);
};

#endif /* INC_COMMANDS_HH_ */
