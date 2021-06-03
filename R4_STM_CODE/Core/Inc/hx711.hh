#ifndef HX711__LIB
#define HX711__LIB
#include <main.h>

class HX711{
	GPIO_TypeDef *Dt_gpio;
	uint16_t Dt_pin;
	GPIO_TypeDef *Sck_gpio;
	uint16_t Sck_pin;
	uint16_t offset;
	uint16_t conversionFactor;
public:
	HX711(GPIO_TypeDef *Dt_gpio, uint16_t Dt_pin, GPIO_TypeDef *Sck_gpio, uint16_t Sck_pin);
	int32_t getWeigthPlusOffsetAfterConversion(uint16_t times = 10);
	void setConversionFactor(uint16_t newConversionFactor);
	void addToOffset(uint16_t offsetDif);
	int32_t Read_Value();
	int32_t Average_Value(uint16_t times);
};

#endif
