#ifndef HX711__LIB
#define HX711__LIB
#include <main.h>

class HX711{
	GPIO_TypeDef *Dt_gpio;
	uint16_t Dt_pin;
	GPIO_TypeDef *Sck_gpio;
	uint16_t Sck_pin;
	int32_t offset;
	int32_t calibrationFactor;
public:
	HX711(GPIO_TypeDef *Dt_gpio, uint16_t Dt_pin, GPIO_TypeDef *Sck_gpio, uint16_t Sck_pin);
	int32_t getWeigthPlusOffsetAfterConversion(uint16_t times = 10);
	int32_t getOffset() const;
	int32_t getCalibrationFactor() const;
	void setCalibrationFactor(int32_t newCalibrationFactor);
	void addToOffset(int32_t offsetDif);

	int32_t Read_Value();
	int32_t Average_Value(uint16_t times);
};

#endif
