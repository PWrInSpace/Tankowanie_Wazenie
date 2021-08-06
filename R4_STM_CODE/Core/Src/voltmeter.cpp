#include "voltmeter.hh"

Voltmeter::Voltmeter(GPIO_TypeDef *_Analog_gpio, uint16_t _Analog_pin, int16_t _VoltageDividerRatio)
: Analog_gpio(_Analog_gpio), Analog_pin(_Analog_pin), VoltageDividerRatio(_VoltageDividerRatio) {}

void Voltmeter::SetVoltageDividerRatio(int16_t NewVoltageDividerRatio){
	VoltageDividerRatio = NewVoltageDividerRatio;
}

int16_t Voltmeter::GetVoltageDividerRatio(){
	return VoltageDividerRatio;
}

int16_t Voltmeter::GetBatteryVoltage(){

}
