#ifndef HX711__LIB
#define HX711__LIB
#include <main.hh>

class HX711{
	GPIO_TypeDef *Dt_gpio;
	uint16_t Dt_pin;
	GPIO_TypeDef *Sck_gpio;
	uint16_t Sck_pin;
	int32_t OffsetInBits;
	int32_t BitsToGramRatio;
	uint8_t GAIN = 1;

public:
	HX711(GPIO_TypeDef *Dt_gpio, uint16_t Dt_pin,
		  GPIO_TypeDef *Sck_gpio, uint16_t Sck_pin);
	int32_t getWeigthInGramsWithOffset(uint16_t times = 10);
	int32_t getOffsetInBits() const;
	int32_t getBitsToGramRatio() const;
	void setBitsToGramRatio(int32_t newCalibrationFactor);
	void initialCalibration(int32_t testLoadInGrams, uint16_t calibrationTime = 10000);
	void tare();
	void setGain(uint8_t gain = 128);
	int32_t ReadValue();
	int32_t AverageValue(uint16_t sampleSize = 20);
};

#endif
