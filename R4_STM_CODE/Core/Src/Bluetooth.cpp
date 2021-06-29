#include <Bluetooth.hh>


Bluetooth_module::Bluetooth_module(UART_HandleTypeDef* _huart)
{
	huart=_huart;
}

////////////////////VARIABLES//////////////////////
char buff[50];
uint8_t timcnt = 0;
uint8_t buffindex = 0;
//////////////////////////////////////////////////







//code to implement inside USART_IRQHANDLER - just paste interrupt function with correct chosen for bluetooth communication

void interrupt_USART(UART_HandleTypeDef *huart) {
	HAL_UART_Receive(huart, (uint8_t*) &buff[buffindex++], 1, 10);
	if (buff[buffindex - 1] == '\n')
		;/////
		//resolveCommand(); // do poprawy cała funkcja/////
}

void interrupt_TIM() {
	if (strlen(buff) > 0) {
		timcnt = 0;
	}
	if (timcnt > 5) {
	//	resolveCommand();
	}
}



void TIM2_IRQHandler(void) {
	/* USER CODE BEGIN TIM2_IRQn 0 */
		if (strlen(buff) > 0) {
			timcnt = 0;
		}
		if (timcnt > 5) {
			resolveCommand(&huart3); // do poprawy
		}

		/* USER CODE END TIM2_IRQn 0 */
		HAL_TIM_IRQHandler(&htim2);
		/* USER CODE BEGIN TIM2_IRQn 1 */
		/* USER CODE END TIM2_IRQn 1 */
}


void USART3_IRQHandler(void) {
	/* USER CODE BEGIN USART3_IRQn 0 */
	HAL_UART_Receive(&huart3, (uint8_t*) &buff[buffindex++], 1, 10);
	if (buff[buffindex - 1] == '\n')
		resolveCommand(&huart3); // do poprawy
	/* USER CODE END USART3_IRQn 0 */
	HAL_UART_IRQHandler(&huart3);
	/* USER CODE BEGIN USART3_IRQn 1 */

	/* USER CODE END USART3_IRQn 1 */
}



bool stringCompare(char array1[], char array2[], uint16_t lght) {
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

void doCommand(Bluetooth_module* Module, Motor *Mot) {

	if (stringCompare(buff,  "TEST_MOTOR", strlen("TEST_MOTOR"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "calibrating the valve \n",
				strlen("calibrating the valve \n"), 500);
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Done... \n",
				strlen("Done... \n"), 500);
		Mot->test_open_close();
		///////////////////////////
	} else if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Opening \n",
				strlen("Opening \n"), 500);
		Mot->open(5);
		//////////////////
	} else if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Closing \n",
				strlen("Closing \n"), 500);
		Mot->close(5);
		//////////////////////////
	} else if (stringCompare(buff, "STOP", strlen("STOP"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Stopped \n",
				strlen("Stopped \n"), 500);
		Mot->stop();
		///////////////
	} else if (stringCompare(buff, "FIRE", strlen("FIRE"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "BOMBS AWAY \n",
				strlen("BOMBS AWAY \n"), 500);
		//igniter_FIRE(igniter);
		//toggle_Mosfet();

	} else {
		HAL_UART_Transmit(Module->huart,
				(uint8_t*) strcat("Wrong command received: ", buff),
				strlen(strcat("Wrong command received: ", buff)), 500);
	}


}
void doCommand1(Bluetooth_module *Module) {

	if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "calibrating the valve \n",
				strlen("calibrating the valve \n"), 500);
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Done... \n",
				strlen("Done... \n"), 500);

		///////////////////////////
	} else if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Opening \n",
				strlen("Opening \n"), 500);

		//////////////////
	} else if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Closing \n",
				strlen("Closing \n"), 500);

		//////////////////////////
	} else if (stringCompare(buff, "STOP", strlen("STOP"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Stopped \n",
				strlen("Stopped \n"), 500);

		///////////////
	} else if (stringCompare(buff, "FIRE", strlen("FIRE"))) {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "BOMBS AWAY \n",
				strlen("BOMBS AWAY \n"), 500);
		//igniter_FIRE(igniter);

	} else {
		HAL_UART_Transmit(Module->huart, (uint8_t*) "^ wrong command received \n",	strlen("^ wrong command received \n"), 500);
	}
}
