#include "../include/components/hx711.h"




float Hx711::CustomCalibration(float known_mass, int delay_ms){

	update();
	tareNoDelay();
	if (getTareStatus() == true) {

		vTaskDelay(delay_ms/portTICK_PERIOD_MS);
		update();
		refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correctly
		float newCalibrationValue = getNewCalibration(known_mass); //get the new calibration value
		return newCalibrationValue;
    }
	return -1;
}


float Hx711::getRawData() // return fresh data from the moving average dataset
{
	long data = 0;
	lastSmoothedData = smoothedData();
	data = lastSmoothedData - tareOffset ;
	float x = (float)data;
	return x;
}