#include <Bluetooth.hh>


Bluetooth::Bluetooth(UART_HandleTypeDef *_huart, Commands _commands) {

	huart = _huart;
	memset(buff, 0, sizeof(buff));
	commands = _commands;

}

//code to implement inside USART_IRQHANDLER - just paste interrupt function with correct chosen for bluetooth communication

void Bluetooth::resolveCommand() {

	HAL_UART_Transmit(huart, (uint8_t*) "in \n", strlen("in \n"), 500); //for debug
	HAL_UART_Transmit(huart, (uint8_t*) buff, strlen(buff), 500); //echo for debug

	// implement a function which will act based on the content of the buffer
	//commands.doCommand_noacc(buff);
	commands.doCommand(buff);
	HAL_UART_Transmit(huart, (uint8_t*) buff, strlen(buff), 500); //echo for debug
	memset(buff, 0, sizeof(buff));
	buffindex = 0;
	timcnt = 0;
	HAL_UART_Transmit(huart, (uint8_t*) "exit \n", strlen("exit \n"), 500); //for debug
}

void Bluetooth::interrupt_USART() {
	HAL_UART_Receive(huart, (uint8_t*) &buff[buffindex++], 1, 10);
	if (buff[buffindex - 1] == '\n')
		this->resolveCommand();
}

//will not be used later
void Bluetooth::interrupt_TIM() {
	if (strlen(buff) > 0) {
		timcnt = 0;
	}
	if (timcnt > 5) {
		this->resolveCommand();

	}
}
