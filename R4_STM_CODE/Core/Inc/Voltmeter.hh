#ifndef INC_VOLTMETER_HH_
#define INC_VOLTMETER_HH_

#include "gpio.h"

class Voltmeter{
	ADC_HandleTypeDef *ADC;
	int16_t VoltageDividerRatio;
public:
	Voltmeter(ADC_HandleTypeDef* _ADC, int16_t _VoltageDividerRatio);
	void SetVoltageDividerRatio(int16_t NewVoltageDividerRatio);
	int16_t GetVoltageDividerRatio();
	int16_t GetBatteryVoltageInMilivolts();
};

#endif /* INC_VOLTMETER_HH_ */
