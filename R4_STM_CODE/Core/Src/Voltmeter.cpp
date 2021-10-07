#include <Voltmeter.hh>

Voltmeter::Voltmeter(ADC_HandleTypeDef* _ADC, int16_t _VoltageDividerRatio): ADC(_ADC), VoltageDividerRatio(_VoltageDividerRatio){
	HAL_ADC_Start(ADC);
}

void Voltmeter::SetVoltageDividerRatio(int16_t NewVoltageDividerRatio){
	VoltageDividerRatio = NewVoltageDividerRatio;
}

int16_t Voltmeter::GetVoltageDividerRatio(){
	return VoltageDividerRatio;
}

int16_t Voltmeter::GetBatteryVoltageInMilivolts(){
	return (int16_t)(HAL_ADC_GetValue(ADC) * 3.21); // Hard Coded
}

float Voltmeter::GetBatteryVoltageInVolts(){
	return (float)HAL_ADC_GetValue(ADC) * 3.21f / 1000.0f; // Hard Coded
}
