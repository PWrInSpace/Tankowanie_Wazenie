#ifndef HX711__LIB
#define HX711__LIB
#include <main.hh>
#include <cmath>

class HX711{
	GPIO_TypeDef *Dt_gpio;
	uint16_t Dt_pin;
	GPIO_TypeDef *Sck_gpio;
	uint16_t Sck_pin;
	int32_t OffsetInBits;
	float BitsToGramRatio;
	uint8_t GAIN;

public:
	HX711(GPIO_TypeDef *Dt_gpio, uint16_t Dt_pin,
		  GPIO_TypeDef *Sck_gpio, uint16_t Sck_pin,
		  int32_t InitialOffsetInBits = 0, float InitialBitsToGramRatio = 0 , uint8_t gain = 64);
	int32_t getWeigthInGramsWithOffset(uint16_t times = 10);
	float getWeigthInKilogramsWithOffset(uint16_t times = 10);
	int32_t getOffsetInBits() const;
	float getBitsToGramRatio() const;
	void setGain(uint8_t gain = 128);
	void setBitsToGramRatio(float newBitsToGramRatio);
	void addToOffset(int32_t DifOffsetInBits);
	void tare();
	void initialCalibration(int32_t testLoadInGrams, uint16_t calibrationTimeInMilis = 6666);
	int8_t waitingForReadyState(uint16_t timeInMilis = 100);
	int32_t ReadValue();
	int32_t AverageValue(uint16_t sampleSize = 20);
	void handleComand(char command, int32_t num);
};

void BlinkNTimesDuringXMilis(uint16_t blinkTimes = 200, uint16_t timeInMilis = 10000);

#endif
