#include <Voltmeter.hh>

Voltmeter::Voltmeter(ADC_HandleTypeDef* ADC_, int16_t VoltageDividerRatio_): ADC(ADC_), VoltageDividerRatio(VoltageDividerRatio_){
	HAL_ADC_Start(ADC);
}

void Voltmeter::SetVoltageDividerRatio(int16_t NewVoltageDividerRatio){
	VoltageDividerRatio = NewVoltageDividerRatio;
}

int16_t Voltmeter::GetVoltageDividerRatio(){
	return VoltageDividerRatio;
}

int16_t Voltmeter::GetVoltageInMilivolts(){
	return (int16_t)(HAL_ADC_GetValue(ADC) * VoltageDividerRatio);
}

float Voltmeter::GetVoltageInVolts(){
	return ((float)GetVoltageInMilivolts()) / 1000.0;
}
