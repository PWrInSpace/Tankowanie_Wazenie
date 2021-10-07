#ifndef HX711__LIB
#define HX711__LIB
#include <main.hh>
#include <cmath>

class HX711{
	GPIO_TypeDef *Dt_gpio;
	uint16_t Dt_pin;
	GPIO_TypeDef *Sck_gpio;
	uint16_t Sck_pin;
	volatile int32_t OffsetInBits;
	volatile float BitsToGramRatio;
	volatile uint8_t Gain;

public:
	HX711(GPIO_TypeDef *Dt_gpio, uint16_t Dt_pin,
		  GPIO_TypeDef *Sck_gpio, uint16_t Sck_pin,
		  int32_t InitialOffsetInBits = 0, float InitialBitsToGramRatio = 0 , uint8_t gain = 64);
	int32_t getWeigthInGramsWithOffset(uint16_t Times = 10);
	float getWeigthInKilogramsWithOffset(uint16_t Times = 10);
	int32_t getOffsetInBits() const;
	float getBitsToGramRatio() const;
	void setGain(uint8_t Gain = 128);
	void setBitsToGramRatio(float NewBitsToGramRatio);
	void addToOffset(float DifOffsetInBits);
	void tare();
	void initialCalibration(float TestLoadInGrams, uint16_t ValibrationTimeInMilis = 6666);
	int8_t waitingForReadyState(uint16_t TimeInMilis = 100);
	int32_t ReadValue();
	int32_t AverageValue(uint16_t SampleSize = 20);
	void handleComand(char Command, float Num);
};

void BlinkNTimesDuringXMilis(uint16_t BlinkTimes = 200, uint16_t TimeInMilis = 10000);

#endif
