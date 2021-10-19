
#include "Hx711.hh"

Hx711::Hx711(GPIO_TypeDef* DataGPIO_, uint16_t DataPin_,
			 GPIO_TypeDef* SerialClockGPIO_, uint16_t SerialClockPin_,
			 int32_t InitialOffsetInBits, float InitialBitsToGramRatio, WeightGain Gain_):
	DataGPIO(DataGPIO_), DataPin(DataPin_), SerialClockGPIO(SerialClockGPIO_), SerialClockPin(SerialClockPin_),
	OffsetInBits(InitialOffsetInBits), BitsToGramRatio(InitialBitsToGramRatio), Gain(Gain_) { }

int32_t Hx711::GetWeigthInGramsWithOffset(uint16_t Times){
	LastRawAverageRead = AverageValue(Times);
	if(std::abs(BitsToGramRatio) > 0.001){ //!= 0
		return (int32_t)((float)(LastRawAverageRead + OffsetInBits) / BitsToGramRatio);
	}
	else //without calibration
		return LastRawAverageRead;
}

float Hx711::GetWeigthInKilogramsWithOffset(uint16_t Times){
	return (float)GetWeigthInGramsWithOffset(Times) / 1000.0f;
}

int32_t Hx711::GetOffsetInBits() const{
	return OffsetInBits;
}

float Hx711::GetBitsToGramRatio() const{
	return BitsToGramRatio;
}

void Hx711::SetBitsToGramRatio(float NewBitsToGramRatio){
	BitsToGramRatio = NewBitsToGramRatio;
}

void Hx711::AddToOffset(float DifferenceOffsetInBits){
	OffsetInBits += (int32_t)DifferenceOffsetInBits;
}

void Hx711::SetGain(WeightGain Gain_){
	Gain = Gain_;
}

void Hx711::Tare(){
	OffsetInBits = -AverageValue();
}

void Hx711::InitialCalibration(float TestLoadInGrams, uint16_t CalibrationTimeInMilis){
	if (TestLoadInGrams < 0)
		return;
	//starts with load cells empty
	int32_t InitialWeight = AverageValue();
	OffsetInBits = -InitialWeight;
	//time to put testLoad on load cell
	BlinkNTimesDuringXMilis(200, CalibrationTimeInMilis);
	int32_t WeightWithLoad = AverageValue();


	BitsToGramRatio = (float)(WeightWithLoad - InitialWeight) / TestLoadInGrams;
}

void Hx711::DoubleCalibration(float FirstTestLoadInGrams, float SecondTestLoadInGrams, uint16_t CalibrationTimeInMilis){
	if (FirstTestLoadInGrams < 0)
		return;
	if (SecondTestLoadInGrams < 0.01){
		InitialCalibration(FirstTestLoadInGrams, CalibrationTimeInMilis);
		return;
	}
	//starts with load cells empty
	int32_t InitialWeight = AverageValue();
	OffsetInBits = -InitialWeight;
	//time to put testLoad on load cell
	BlinkNTimesDuringXMilis(200, CalibrationTimeInMilis);
	int32_t WeightWithFirstLoad = AverageValue();
	//time to put secondLoad on load cell
	BlinkNTimesDuringXMilis(40, CalibrationTimeInMilis);
	int32_t WeightWithSecondLoad = AverageValue();

	float FirstBitsToGramRatio = (float)(WeightWithFirstLoad - InitialWeight) / FirstTestLoadInGrams;
	float SecondBitsToGramRatio = (float)(WeightWithSecondLoad - InitialWeight) / SecondTestLoadInGrams;
	BitsToGramRatio = (FirstBitsToGramRatio + SecondBitsToGramRatio) / 2.0f;
}


int32_t Hx711::ReadValue(){
	HAL_GPIO_WritePin(SerialClockGPIO, SerialClockPin, GPIO_PIN_RESET);
	int32_t Buffer = 0;

	if (!WaitingForReadyState())
		return 0;
	for (uint8_t i = 0; i < 24; ++i){
		HAL_GPIO_WritePin(SerialClockGPIO, SerialClockPin, GPIO_PIN_SET);
		Buffer = Buffer << 1 ;
		Buffer+=HAL_GPIO_ReadPin(DataGPIO, DataPin);
		HAL_GPIO_WritePin(SerialClockGPIO, SerialClockPin, GPIO_PIN_RESET);
	}
	for(uint8_t i = 0; i < Gain; ++i){
		HAL_GPIO_WritePin(SerialClockGPIO, SerialClockPin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SerialClockGPIO, SerialClockPin, GPIO_PIN_RESET);
	}
	WaitingForReadyState();

    if (Buffer & 0x800000){
    	Buffer |= 0xFF000000;
    }

	return Buffer;
}

int32_t Hx711::AverageValue(uint16_t SampleSize){
	int64_t Sum = 0;
	int32_t Read = 0;
	int16_t SuccesfulReads = 0;
    for (uint16_t i = 0; i < SampleSize; ++i){
    	Read = ReadValue();
    	if(Read != 0){
    	    Sum += Read;
    		++SuccesfulReads;
    	}
    }
    if(SuccesfulReads > 0)
    	return (int32_t)(Sum / SuccesfulReads);
    else
    	return -1;
}

int8_t Hx711::WaitingForReadyState(uint16_t TimeInMilis){
	for(uint16_t i = 0; i < TimeInMilis; ++i){
		HAL_Delay(1);
		if(HAL_GPIO_ReadPin(DataGPIO, DataPin) == 0 ) //ready
			return 1;
		else
			continue;
	}
	return 0;
}

void Hx711::WeightCommandHandler(char Command, float InputNumber){
	if(Command == 'C')
		InitialCalibration(InputNumber);
	else if(Command == 'T')
		Tare();
	else if(Command == 'R')
		SetBitsToGramRatio(InputNumber);
	else if(Command == 'O')
		AddToOffset(InputNumber);
}

void Hx711::TEMPWeightCommandHandler(char Command, float InputNumber, float SecondInputNumber){
	if(Command == 'C')
		DoubleCalibration(InputNumber, SecondInputNumber);
	else if(Command == 'T')
		Tare();
	else if(Command == 'R')
		SetBitsToGramRatio(InputNumber);
	else if(Command == 'O')
		AddToOffset(InputNumber);
}

void BlinkNTimesDuringXMilis(uint16_t BlinkTimes, uint16_t TimeInMilis){
	for(uint16_t i = 0; i < BlinkTimes ; ++i){
		HAL_GPIO_TogglePin(BUILD_IN_LED_GPIO_Port, BUILD_IN_LED_Pin);
		HAL_Delay(TimeInMilis / BlinkTimes);
	}
	HAL_GPIO_WritePin(BUILD_IN_LED_GPIO_Port, BUILD_IN_LED_Pin, GPIO_PIN_SET);
}
