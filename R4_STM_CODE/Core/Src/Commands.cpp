#include <Commands.hh>

/*
 * commands.cpp
 *
 *  Created on: 30 cze 2021
 *      Author: KooBeK
 */

Commands::Commands(){
fill = new Motor(FILL_OPEN_GPIO_Port, FILL_OPEN_Pin,	FILL_CLOSE_GPIO_Port, FILL_CLOSE_Pin, &htim4, TIM_CHANNEL_3,
						FILL_O_LIMIT_SW_GPIO_Port, FILL_O_LIMIT_SW_Pin,	FILL_C_LIMIT_SW_GPIO_Port, FILL_C_LIMIT_SW_Pin);

depr = new Motor(DEPR_OPEN_GPIO_Port, DEPR_OPEN_Pin,	DEPR_CLOSE_GPIO_Port, DEPR_CLOSE_Pin, &htim4, TIM_CHANNEL_3,
		DEPR_O_LIMIT_SW_GPIO_Port, DEPR_O_LIMIT_SW_Pin,	DEPR_C_LIMIT_SW_GPIO_Port, DEPR_C_LIMIT_SW_Pin);

}

Motor* Commands::getFill() {
	return fill;
}

Motor* Commands::getDepr() {
	return depr;
}
Motor* Commands::getQuickRel() {
	//return quickrel;

}

bool stringCompare(const char array1[], const char array2[], uint16_t lght) {
	uint8_t var = 0;

	for (int i = 0; i < lght; i++) {
		if (array1[i] == array2[i]) {
			var++;
		} else
			var = 0;
	}
	if (var == lght)
		return 1;
	else
		return 0;
}


void Commands::doCommand(char buff[]) {
	char num = buff[0];

		for (int i = 0; i < 49; i++) {
			buff[i] = buff[i + 1];
		}


	switch (num) {
	case '1':
		if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
			HAL_UART_Transmit(huart,
					(uint8_t*) "calibrating the valve \n",
					strlen("calibrating the valve \n"), 500);
			HAL_UART_Transmit(huart, (uint8_t*) "Done... \n",
					strlen("Done... \n"), 500);
			fill->test_open_close();

			///////////////////////////
		} else if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
			HAL_UART_Transmit(huart, (uint8_t*) "Opening \n",
					strlen("Opening \n"), 500);
			fill->open(5);
			//////////////////
		} else if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
			HAL_UART_Transmit(huart, (uint8_t*) "Closing \n",
					strlen("Closing \n"), 500);
			fill->close(5);
			//////////////////////////
		} else if (stringCompare(buff, "STOP", strlen("STOP"))) {
			HAL_UART_Transmit(huart, (uint8_t*) "Stopped \n",
					strlen("Stopped \n"), 500);
			fill->stop();
			///////////////
		} else if (stringCompare(buff, "FIRE", strlen("FIRE"))) {
			HAL_UART_Transmit(huart, (uint8_t*) "BOMBS AWAY \n",
					strlen("BOMBS AWAY \n"), 500);
			//igniter_FIRE(igniter);
			//

		} else {
			HAL_UART_Transmit(huart,
					(uint8_t*) "^ wrong command received \n",
					strlen("^ wrong command received \n"), 500);
		}
		break;
	case '2':
		if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
			HAL_UART_Transmit(huart,
					(uint8_t*) "calibrating the valve \n",
					strlen("calibrating the valve \n"), 500);
			HAL_UART_Transmit(huart, (uint8_t*) "Done... \n",
					strlen("Done... \n"), 500);
			depr->test_open_close();
			///////////////////////////
		} else if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
			HAL_UART_Transmit(huart, (uint8_t*) "Opening \n",
					strlen("Opening \n"), 500);
			depr->open(5);
			//////////////////
		} else if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
			HAL_UART_Transmit(huart, (uint8_t*) "Closing \n",
					strlen("Closing \n"), 500);
			depr->close(5);
			//////////////////////////
		} else if (stringCompare(buff, "STOP", strlen("STOP"))) {
			HAL_UART_Transmit(huart, (uint8_t*) "Stopped \n",
					strlen("Stopped \n"), 500);
			depr->stop();
			///////////////
		} else if (stringCompare(buff, "FIRE", strlen("FIRE"))) {
			HAL_UART_Transmit(huart, (uint8_t*) "BOMBS AWAY \n",
					strlen("BOMBS AWAY \n"), 500);
			//igniter_FIRE(igniter);
			//

		} else {
			HAL_UART_Transmit(huart,
					(uint8_t*) "^ wrong command received \n",
					strlen("^ wrong command received \n"), 500);
		}
		break;
	default:
		break;
	}
}




void Commands::doCommand_noacc(char buff[]) {
	char num = buff[0];

	for (int i = 0; i < 49; i++) {
		buff[i] = buff[i + 1];
	}

	switch (num) {
	case '1':
		if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "calibrating the valve \n",
					strlen("calibrating the valve \n"), 500);

			HAL_UART_Transmit(&huart3, (uint8_t*) "Done... \n",
					strlen("Done... \n"), 500);

			///////////////////////////
		} else if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "Opening \n",
					strlen("Opening \n"), 500);

			//////////////////
		} else if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "Closing \n",
					strlen("Closing \n"), 500);

			//////////////////////////
		} else if (stringCompare(buff, "STOP", strlen("STOP"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "Stopped \n",
					strlen("Stopped \n"), 500);

			///////////////
		} else if (stringCompare(buff, "FIRE", strlen("FIRE"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "BOMBS AWAY \n",
					strlen("BOMBS AWAY \n"), 500);

		} else {
			HAL_UART_Transmit(&huart3, (uint8_t*) "^ wrong command received \n",
					strlen("^ wrong command received \n"), 500);
		}

		break;
	case '2':
		if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "calibrating the valve \n",
					strlen("calibrating the valve \n"), 500);
			HAL_UART_Transmit(&huart3, (uint8_t*) "Done... \n",
					strlen("Done... \n"), 500);
			///////////////////////////
		} else if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "Opening \n",
					strlen("Opening \n"), 500);
			//////////////////
		} else if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "Closing \n",
					strlen("Closing \n"), 500);
			//////////////////////////
		} else if (stringCompare(buff, "STOP", strlen("STOP"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "Stopped \n",
					strlen("Stopped \n"), 500);
			///////////////
		} else if (stringCompare(buff, "FIRE", strlen("FIRE"))) {
			HAL_UART_Transmit(&huart3, (uint8_t*) "BOMBS AWAY \n",
					strlen("BOMBS AWAY \n"), 500);
		} else {
			HAL_UART_Transmit(&huart3, (uint8_t*) "^ wrong command received \n",
					strlen("^ wrong command received \n"), 500);
		}
		break;
	default:
		HAL_UART_Transmit(&huart3, (uint8_t*) "^ wrong command format \n",
				strlen("^ wrong command format \n"), 500);
		break;
	}
}

