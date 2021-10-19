#include <Bluetooth.hh>


Bluetooth::Bluetooth(UART_HandleTypeDef *_huart, Rocket _rocket) {

	huart = _huart;
	memset(buff, 0, sizeof(buff));
	rocket = _rocket;

}

//code to implement inside USART_IRQHANDLER - just paste interrupt function with correct chosen for bluetooth communication

void Bluetooth::resolveCommand() {
	char* tmp = "in \n";
	HAL_UART_Transmit(huart, (uint8_t*)tmp , (uint16_t)strlen(tmp), 500); //for debug
	HAL_UART_Transmit(huart, (uint8_t*) buff, (uint16_t)strlen(buff), 500); //echo for debug

	// implement a function which will act based on the content of the buffer
	//commands.doCommand_noacc(buff);
	rocket.RocketCommandHandler(buff);
	HAL_UART_Transmit(huart, (uint8_t*) buff, (uint16_t)strlen(buff), 500); //echo for debug
	memset(buff, 0, sizeof(buff));
	buffindex = 0;
	timcnt = 0;
	tmp = "exit \n";
	HAL_UART_Transmit(huart, (uint8_t*)tmp, (uint16_t)strlen("exit \n"), 500); //for debug
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
