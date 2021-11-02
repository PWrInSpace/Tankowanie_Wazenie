#ifndef HX711__LIB
#define HX711__LIB

#include <main.hh>
#include <cmath>
#include "weight.hh"

enum WeightGain {Gain128 = 1, Gain64 = 3};

class Hx711: public Weight{
	GPIO_TypeDef *DataGPIO;
	uint16_t DataPin;
	GPIO_TypeDef *SerialClockGPIO;
	uint16_t SerialClockPin;
	volatile int32_t OffsetInBits;
	volatile float BitsToGramRatio;
	volatile WeightGain Gain;

	int8_t WaitingForReadyState(const uint16_t Timeout = 100);
	int32_t ReadValue();
	int32_t AverageValue(uint16_t SampleSize = 20);
public:
	volatile int32_t LastRawAverageRead;

	Hx711(GPIO_TypeDef *Dt_gpio, uint16_t Dt_pin,
		  GPIO_TypeDef *Sck_gpio, uint16_t Sck_pin,
		  int32_t InitialOffsetInBits = 0, float InitialBitsToGramRatio = 0 , WeightGain Gain_ = WeightGain::Gain64);
	int32_t GetWeigthInGramsWithOffset(uint16_t Times = 20);
	float GetWeigthInKilogramsWithOffset(uint16_t Times = 20);
	int32_t GetOffsetInBits() const;
	float GetBitsToGramRatio() const;
	void SetGain(WeightGain Gain_);
	void SetBitsToGramRatio(float NewBitsToGramRatio);
	void AddToOffset(float DifferenceOffsetInBits);
	void Tare() override;
	void InitialCalibration(float TestLoadInGrams, const uint16_t CalibrationTimeInMilis = 6666) override;
	void DoubleCalibration(float TestLoadInGrams, float SecondTestLoadInGrams, const uint16_t CalibrationTimeInMilis = 6666);
	void WeightCommandHandler(char Command, float Number) override;
	void TEMPWeightCommandHandler(char Command, float InputNumber, float SecondInputNumber);
};

void BlinkNTimesDuringXMilis(const uint16_t BlinkTimes = 200, const uint16_t TimeInMilis = 10000);
#endif
