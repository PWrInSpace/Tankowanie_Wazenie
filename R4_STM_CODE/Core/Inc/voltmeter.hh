#ifndef INC_VOLTMETER_HH_
#define INC_VOLTMETER_HH_

#include "gpio.h"

class Voltmeter{
	GPIO_TypeDef *Analog_gpio;
	uint16_t Analog_pin;
	int16_t VoltageDividerRatio;
public:
	Voltmeter(GPIO_TypeDef *_Analog_gpio, uint16_t _Analog_pin, int16_t _VoltageDividerRatio);
	void SetVoltageDividerRatio(int16_t NewVoltageDividerRatio);
	int16_t GetVoltageDividerRatio();
	int16_t GetBatteryVoltage();
};

#endif /* INC_VOLTMETER_HH_ */
