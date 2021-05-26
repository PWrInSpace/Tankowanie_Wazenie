#include <Bluetooth.h>

////////////////////VARIABLES//////////////////////
char buff[50];
uint8_t timcnt = 0;
uint8_t buffindex = 0;
//////////////////////////////////////////////////

Bluetooth_module* bluetooth_init(UART_HandleTypeDef *_huart) {

	Bluetooth_module *module = malloc(sizeof(Bluetooth_module));

	module->huart = _huart;

	return module;
}
;

//code to implement inside USART_IRQHANDLER - just paste interrupt function with correct chosen for bluetooth communication

void interrupt(UART_HandleTypeDef *huart) {
	HAL_UART_Receive(&huart, (uint8_t*) &buff[buffindex++], 1, 10);
	if (buff[buffindex - 1] == '\n')
		resolveCommand(); // do poprawy
}

void resolveCommand(Bluetooth_module *Module) {

	HAL_UART_Transmit(Module->huart, (uint8_t*) "in \n", strlen("in \n"), 500);
	HAL_UART_Transmit(Module->huart, (uint8_t*) buff, strlen(buff), 500);

	switch (whateva()) {
	case 1:
		HAL_UART_Transmit(Module->huart, (uint8_t*) "calibrating the valve \n",
				strlen("calibrating the valve \n"), 500);
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Done... \n",
				strlen("Done... \n"), 500);
		//
	case 2:
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Opening \n",
				strlen("Opening \n"), 500);
		//motor_opening(Mot);
	case 3:
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Closing \n",
				strlen("Closing \n"), 500);
		//
	case 4:
		HAL_UART_Transmit(Module->huart, (uint8_t*) "Stopped \n",
				strlen("Stopped \n"), 500);
	default :
		HAL_UART_Transmit(Module->huart, (uint8_t*) strcat("Wrong command received: ",buff),
						strlen(strcat("Wrong command received: ",buff)), 500);
	}

	memset(buff, 0, sizeof(buff));
	buffindex = 0;
	timcnt = 0;
	HAL_UART_Transmit(Module->huart, (uint8_t*) "exit \n", strlen("exit \n"),
			500);
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

int whateva() {
	if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
		return 1;
	}
	if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
		return 2;
	}
	if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
		return 3;
	}
	if (stringCompare(buff, "STOP", strlen("STOP"))) {
		return 4;
	}
	else return 0;
}
