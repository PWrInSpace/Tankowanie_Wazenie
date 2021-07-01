#include <commands.h>

/*
 * commands.cpp
 *
 *  Created on: 30 cze 2021
 *      Author: KooBeK
 */

void doCommand(Bluetooth_module *Module, Motor *Mot, Motor *Depr,
		Motor *Quickrel) {
	char num = buff[0];

	for(int i=0;i<49;i++)
	{
		buff[i] = buff[i+1];
	}

	switch (num) {
	case '1':
		if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
			HAL_UART_Transmit(Module->huart,
					(uint8_t*) "calibrating the valve \n",
					strlen("calibrating the valve \n"), 500);
			HAL_UART_Transmit(Module->huart, (uint8_t*) "Done... \n",
					strlen("Done... \n"), 500);
			motor_initial(Mot);
			///////////////////////////
		} else if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
			HAL_UART_Transmit(Module->huart, (uint8_t*) "Opening \n",
					strlen("Opening \n"), 500);
			motor_opening(Mot);
			//////////////////
		} else if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
			HAL_UART_Transmit(Module->huart, (uint8_t*) "Closing \n",
					strlen("Closing \n"), 500);
			motor_closing(Mot);
			//////////////////////////
		} else if (stringCompare(buff, "STOP", strlen("STOP"))) {
			HAL_UART_Transmit(Module->huart, (uint8_t*) "Stopped \n",
					strlen("Stopped \n"), 500);
			motor_stop(Mot);
			///////////////
		} else if (stringCompare(buff, "FIRE", strlen("FIRE"))) {
			HAL_UART_Transmit(Module->huart, (uint8_t*) "BOMBS AWAY \n",
					strlen("BOMBS AWAY \n"), 500);
			//igniter_FIRE(igniter);
			toggle_Mosfet();

		} else {
			HAL_UART_Transmit(Module->huart,
					(uint8_t*) "^ wrong command received \n",
					strlen("^ wrong command received \n"), 500);
		}

		break;
	case '2':
		if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
					HAL_UART_Transmit(Module->huart,
							(uint8_t*) "calibrating the valve \n",
							strlen("calibrating the valve \n"), 500);
					HAL_UART_Transmit(Module->huart, (uint8_t*) "Done... \n",
							strlen("Done... \n"), 500);
					motor_initial(Depr);
					///////////////////////////
				} else if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
					HAL_UART_Transmit(Module->huart, (uint8_t*) "Opening \n",
							strlen("Opening \n"), 500);
					motor_opening(Depr);
					//////////////////
				} else if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
					HAL_UART_Transmit(Module->huart, (uint8_t*) "Closing \n",
							strlen("Closing \n"), 500);
					motor_closing(Depr);
					//////////////////////////
				} else if (stringCompare(buff, "STOP", strlen("STOP"))) {
					HAL_UART_Transmit(Module->huart, (uint8_t*) "Stopped \n",
							strlen("Stopped \n"), 500);
					motor_stop(Depr);
					///////////////
				} else if (stringCompare(buff, "FIRE", strlen("FIRE"))) {
					HAL_UART_Transmit(Module->huart, (uint8_t*) "BOMBS AWAY \n",
							strlen("BOMBS AWAY \n"), 500);
					//igniter_FIRE(igniter);
					toggle_Mosfet();

				} else {
					HAL_UART_Transmit(Module->huart,
							(uint8_t*) "^ wrong command received \n",
							strlen("^ wrong command received \n"), 500);
				}

				break;
	case '3':
		if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
							HAL_UART_Transmit(Module->huart,
									(uint8_t*) "calibrating the valve \n",
									strlen("calibrating the valve \n"), 500);
							HAL_UART_Transmit(Module->huart, (uint8_t*) "Done... \n",
									strlen("Done... \n"), 500);
							motor_initial(Depr);
							///////////////////////////
						} else if (stringCompare(buff, "OPEN", strlen("OPEN"))) {
							HAL_UART_Transmit(Module->huart, (uint8_t*) "Opening \n",
									strlen("Opening \n"), 500);
							motor_opening(Depr);
							//////////////////
						} else if (stringCompare(buff, "CLOSE", strlen("CLOSE"))) {
							HAL_UART_Transmit(Module->huart, (uint8_t*) "Closing \n",
									strlen("Closing \n"), 500);
							motor_closing(Depr);
							//////////////////////////
						} else if (stringCompare(buff, "STOP", strlen("STOP"))) {
							HAL_UART_Transmit(Module->huart, (uint8_t*) "Stopped \n",
									strlen("Stopped \n"), 500);
							motor_stop(Depr);
							///////////////
						} else if (stringCompare(buff, "FIRE", strlen("FIRE"))) {
							HAL_UART_Transmit(Module->huart, (uint8_t*) "BOMBS AWAY \n",
									strlen("BOMBS AWAY \n"), 500);
							//igniter_FIRE(igniter);
							toggle_Mosfet();

						} else {
							HAL_UART_Transmit(Module->huart,
									(uint8_t*) "^ wrong command received \n",
									strlen("^ wrong command received \n"), 500);
						}

						break;
	default:
		break;
	}
}

void doCommand_noacc(Bluetooth_module *Module) {
	char num = buff[0];
	for(int i=0;i<49;i++)
	{
		buff[i] = buff[i+1];
	}
	switch (num) {
	case '1':
		if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
			HAL_UART_Transmit(Module->huart,
					(uint8_t*) "calibrating the valve \n",
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
			HAL_UART_Transmit(Module->huart,
					(uint8_t*) "^ wrong command received \n",
					strlen("^ wrong command received \n"), 500);
		}

		break;
	case '2':
		if (stringCompare(buff, "TEST_MOTOR", strlen("TEST_MOTOR"))) {
					HAL_UART_Transmit(Module->huart,
							(uint8_t*) "calibrating the valve \n",
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
					HAL_UART_Transmit(Module->huart,
							(uint8_t*) "^ wrong command received \n",
							strlen("^ wrong command received \n"), 500);
				}
				break;
	case '3':
		break;
	default:
		break;
	}
}


