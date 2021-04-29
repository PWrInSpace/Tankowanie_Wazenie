#ifndef HX711
	#define HX711
	#include <main.h>

	int32_t HX711_Read_Value(GPIO_TypeDef *Dt_gpio, uint16_t Dt_pin, GPIO_TypeDef *Sck_gpio, uint16_t Sck_pin);
	int32_t HX711_Average_Value(GPIO_TypeDef *Dt_gpio, uint16_t Dt_pin, GPIO_TypeDef *Sck_gpio, uint16_t Sck_pin, uint8_t times);
#endif
