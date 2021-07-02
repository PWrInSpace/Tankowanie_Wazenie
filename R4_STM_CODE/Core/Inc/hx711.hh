#ifndef HX711__LIB
#define HX711__LIB
#include <main.h>

class HX711{
	GPIO_TypeDef *Dt_gpio;
	uint16_t Dt_pin;
	GPIO_TypeDef *Sck_gpio;
	uint16_t Sck_pin;
	int32_t OffsetInGrams;
	int32_t BitsToGramRatio;

public:
	HX711(GPIO_TypeDef *Dt_gpio, uint16_t Dt_pin,
		  GPIO_TypeDef *Sck_gpio, uint16_t Sck_pin);
	int32_t getWeigthInGramsWithOffset(uint16_t times = 10);
	//int32_t getOffsetInGrams() const;
	int32_t getBitsToGramRatio() const;
	void setBitsToGramRatio(int32_t newCalibrationFactor);
	//void addToOffsetInGrams(int32_t offsetDif);
	void tare(uint16_t times = 10);
	int32_t ReadValue();
	int32_t AverageValue(uint16_t times = 10);
};

#endif
