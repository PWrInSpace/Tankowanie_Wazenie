#include <Bluetooth.h>
#include <L298.hh>


////////////////////VARIABLES//////////////////////
char buff [50];
uint8_t timcnt=0;
uint8_t buffindex=0;
//////////////////////////////////////////////////


void resolveCommand(UART_HandleTypeDef* huart)
{
	HAL_UART_Transmit(huart, (uint8_t*)"in \n",strlen("in \n"), 500);
	HAL_UART_Transmit(huart, (uint8_t*)buff,strlen(buff), 500);

	if(stringCompare(buff,"INIT_V0", strlen("INIT_V0"))){

		HAL_UART_Transmit(huart, (uint8_t*)"calibrating main valve \n",strlen("calibrating main valve \n"), 500);
		//String bufor = buff;
		HAL_UART_Transmit(huart, (uint8_t*)"Done... \n",strlen("Done... \n"), 500);
	}
	if(stringCompare(buff,"OPEN", strlen("OPEN"))){
		HAL_UART_Transmit(huart, (uint8_t*)"Opening \n",strlen("Opening \n"), 500);
		//motor_opening(Mot);
	}


	if(stringCompare(buff,"CLOSE", strlen("CLOSE")))
	{
		HAL_UART_Transmit(huart, (uint8_t*)"Closing \n",strlen("Closing \n"), 500);
	}

		//motor_closing(Mot);
	//else
	//if()
	//the rest of intruction go here below	
		//in order to transmit data to the device via blueotooth
		// HAL_UART_Transmit(&huart2, stringOrSth, Size, 500);
	
		//then just clear the buffer
	//this line added to test the push to github
	//

		memset(buff,0,sizeof(buff));
		buffindex = 0;
		timcnt = 0;
		HAL_UART_Transmit(huart, (uint8_t*)"exit \n",strlen("exit \n"), 500);
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
