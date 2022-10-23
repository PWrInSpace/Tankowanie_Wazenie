#include "../include/components/hx711.h"

float Hx711::CustomCalibration(float known_mass)
{
	//Serial.println("WAIT FOR TARE");

	/*refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correctly
	//tare();
	uint32_t initialValue = smoothedData();
	setTareOffset(initialValue);

	Serial.println("put");
	vTaskDelay(delay_ms / portTICK_PERIOD_MS);
	refreshDataSet();
	uint32_t finalValue = smoothedData();
	setCalFactor(((float)(finalValue-initialValue)/known_mass));*/

  //tare();



  getNewCalibration(known_mass);


	return getCalFactor();
	
}

float Hx711::getRawData() // return fresh data from the moving average dataset
{
	long data = 0;
	lastSmoothedData = smoothedData();
	data = lastSmoothedData - tareOffset;
	float x = (float)data;
	return x;
}