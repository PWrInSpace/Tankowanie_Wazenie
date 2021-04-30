#include <Bluetooth.h>
#include <L298.h>

void resolveCommand(UART_HandleTypeDef* huart1, Motor* Mot){
	char buff [50];

	if(stringCompare(buff,"INIT_V0", strlen("INIT_V0"))){
		HAL_UART_Transmit(huart1, (uint8_t*)"calibrating main valve \n",strlen("calibrating main valve \n"), 500);
		//String bufor = buff;
		motor_initial(Mot);
		HAL_UART_Transmit(huart1, (uint8_t*)"Done... \n",strlen("Done... \n"), 500);
	}
	if(stringCompare(buff,"OPEN", strlen("OPEN")))
		motor_opening(Mot);
	if(stringCompare(buff,"CLOSE", strlen("CLOSE")))
		motor_closing(Mot);
	//else
	//if()
	//the rest of intruction go here below	
		//in order to transmit data to the device via blueotooth
		// HAL_UART_Transmit(&huart2, stringOrSth, Size, 500);
	
		//then just clear the buffer
	//this line added to test the push to github
	//
		memset(buff,0,sizeof(buff));
}

bool stringCompare(char array1[], char array2[], uint16_t lght){
	uint8_t var = 0;

	for(int i=0;i<lght;i++){
		if(array1[i]==array2[i]){
			var++;
		}
		else var=0;
	}
	if(var==lght)
		return 1;
	else 
		return 0;
}
