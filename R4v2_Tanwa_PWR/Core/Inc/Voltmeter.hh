#ifndef INC_VOLTMETER_HH_
#define INC_VOLTMETER_HH_

#include "stm32f1xx_hal.h"
class Voltmeter{
	ADC_HandleTypeDef *ADC;
	int16_t VoltageDividerRatio;
public:
	Voltmeter(ADC_HandleTypeDef* ADC_, int16_t VoltageDividerRatio_);
	void SetVoltageDividerRatio(int16_t NewVoltageDividerRatio);
	int16_t GetVoltageDividerRatio();
	int16_t GetVoltageInMilivolts();
	float GetVoltageInVolts();
};

#endif /* INC_VOLTMETER_HH_ */
