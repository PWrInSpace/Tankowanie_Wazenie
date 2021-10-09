#include <hx711.hh>

HX711::HX711(GPIO_TypeDef* _Dt_gpio, uint16_t _Dt_pin,
			 GPIO_TypeDef* _Sck_gpio, uint16_t _Sck_pin,
			 int32_t InitialOffsetInBits, float InitialBitsToGramRatio, uint8_t Gain):
	Dt_gpio(_Dt_gpio), Dt_pin(_Dt_pin), Sck_gpio(_Sck_gpio), Sck_pin(_Sck_pin),
	OffsetInBits(InitialOffsetInBits), BitsToGramRatio(InitialBitsToGramRatio)
{
	setGain(Gain);
}

int32_t HX711::getWeigthInGramsWithOffset(uint16_t times){
	int32_t average = AverageValue(times);
	if(std::abs(BitsToGramRatio) > 0.001){ //!= 0
		return (int32_t)((float)(average + OffsetInBits) / BitsToGramRatio);
	}
	else //without calibration
		return average;
}

float HX711::getWeigthInKilogramsWithOffset(uint16_t times){
	return (float)getWeigthInGramsWithOffset(times) / 1000.0f;
}

int32_t HX711::getOffsetInBits() const{
	return OffsetInBits;
}

float HX711::getBitsToGramRatio() const{
	return BitsToGramRatio;
}

void HX711::setBitsToGramRatio(float newBitsToGramRatio){
	BitsToGramRatio = newBitsToGramRatio;
}

void HX711::addToOffset(float DifOffsetInBits){
	OffsetInBits += (int32_t)DifOffsetInBits;
}


void HX711::setGain(uint8_t Gain){
	switch (Gain) {
		case 128:		// channel A, Gain factor 128
			Gain = 1;
			break;
		case 64:		// channel A, Gain factor 64
			Gain = 3;
			break;
		case 32:		// channel B, Gain factor 32
			Gain = 2;
			break;
	}
}

void HX711::tare(){
	OffsetInBits = -AverageValue();
}

void HX711::initialCalibration(float testLoadInGrams, uint16_t calibrationTimeInMilis){
	if (testLoadInGrams < 0)
		return;
	//starts with load cells empty
	int32_t initialWeight = AverageValue();
	OffsetInBits = -initialWeight;
	//time to put testLoad on load cell
	BlinkNTimesDuringXMilis(200, calibrationTimeInMilis);
	int32_t weightWithLoad = AverageValue();

	BitsToGramRatio = (float)(weightWithLoad - initialWeight) / testLoadInGrams;
}

int32_t HX711::ReadValue(){
	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
	int32_t buffer = 0;

	if (!waitingForReadyState())
		return 0;
	for (uint8_t i = 0; i < 24; ++i){
		HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
		buffer = buffer << 1 ;
		buffer+=HAL_GPIO_ReadPin(Dt_gpio, Dt_pin);
		HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
	}
	for(uint8_t i = 0; i < Gain; ++i){
		HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
	}
	waitingForReadyState();

    if (buffer & 0x800000){
    	buffer |= 0xFF000000;
    }
	return buffer;
}

int32_t HX711::AverageValue(uint16_t SampleSize){
	int64_t sum = 0;
	int32_t read = 0;
	int16_t succesfulReads = 0;
    for (uint16_t i = 0; i < SampleSize; ++i){
    	read = ReadValue();
    	if(read != 0){
    	    sum += read;
    		++succesfulReads;
    	}
    }
    if(succesfulReads > 0)
    	return (int32_t)(sum / succesfulReads);
    else
    	return -1;
}


int8_t HX711::waitingForReadyState(uint16_t TimeInMilis){
	for(uint16_t i = 0; i < TimeInMilis; ++i){
		HAL_Delay(1);
		if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 0 ) //ready
			return 1;
		else
			continue;
	}
	return 0;
}

void HX711::WeightCommandHandler(char Command, float InputNumber){
	if(Command == 'C')
		initialCalibration(InputNumber);
	else if(Command == 'T')
		tare();
	else if(Command == 'R')
		setBitsToGramRatio(InputNumber);
	else if(Command == 'O')
		addToOffset(InputNumber);
	}

void BlinkNTimesDuringXMilis(uint16_t BlinkTimes, uint16_t TimeInMilis){
	for(uint16_t i = 0; i < BlinkTimes ; ++i){
		HAL_GPIO_TogglePin(BUILD_IN_LED_GPIO_Port, BUILD_IN_LED_Pin);
		HAL_Delay(TimeInMilis / BlinkTimes);
	}
	HAL_GPIO_WritePin(BUILD_IN_LED_GPIO_Port, BUILD_IN_LED_Pin, GPIO_PIN_SET);
}