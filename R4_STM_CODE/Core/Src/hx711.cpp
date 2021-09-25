#include <hx711.hh>

HX711::HX711(GPIO_TypeDef* _Dt_gpio, uint16_t _Dt_pin,
			 GPIO_TypeDef* _Sck_gpio, uint16_t _Sck_pin, uint8_t gain):
	Dt_gpio(_Dt_gpio), Dt_pin(_Dt_pin), Sck_gpio(_Sck_gpio), Sck_pin(_Sck_pin), BitsToGramRatio(0)
{
	setGain(gain);
}

int32_t HX711::getWeigthInGramsWithOffset(uint16_t times){
	int32_t average = AverageValue(times);
	if(BitsToGramRatio != 0){
		return (average + OffsetInBits) / BitsToGramRatio;
	}
	else //without calibration
		return average;
}

float HX711::getWeigthInKilogramsWithOffset(uint16_t times){
	 float tmp = ((float)getWeigthInGramsWithOffset(times)) / 1000.0;
	 return tmp;
}

int32_t HX711::getOffsetInBits() const{
	return OffsetInBits;
}

int32_t HX711::getBitsToGramRatio() const{
	return BitsToGramRatio;
}

void HX711::setBitsToGramRatio(int32_t newBitsToGramRatio){
	BitsToGramRatio = newBitsToGramRatio;
}

void HX711::addToOffset(int32_t DifOffsetInBits){
	OffsetInBits += DifOffsetInBits;
}


void HX711::setGain(uint8_t gain){
	switch (gain) {
		case 128:		// channel A, gain factor 128
			GAIN = 1;
			break;
		case 64:		// channel A, gain factor 64
			GAIN = 3;
			break;
		case 32:		// channel B, gain factor 32
			GAIN = 2;
			break;
	}
}

void HX711::tare(){
	OffsetInBits = -AverageValue();
}

void HX711::initialCalibration(int32_t testLoadInGrams, uint16_t calibrationTimeInMilis){
	if (testLoadInGrams == 0)
		return;
	//starts with load cells empty
	int32_t initialWeight = AverageValue();
	OffsetInBits = -initialWeight;
	//time to put testLoad on load cell
	BlinkNTimesDuringXMilis(200, calibrationTimeInMilis);
	int32_t weightWithLoad = AverageValue();

	BitsToGramRatio = (weightWithLoad - initialWeight) / testLoadInGrams;
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
	for(uint8_t i = 0; i < GAIN; ++i){
		HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
	}
	waitingForReadyState();

    if (buffer & 0x800000){
    	buffer |= 0xFF000000;
    }
	return buffer;
}

int32_t HX711::AverageValue(uint16_t sampleSize){
	int64_t sum = 0;
	int32_t read = 0;
	int16_t succesfulReads = 0;
    for (uint16_t i = 0; i < sampleSize; ++i){
    	read = ReadValue();
    	if(read != 0){
    	    sum += read;
    		++succesfulReads;
    	}
    }
    if(succesfulReads > 0)
    	return sum / (succesfulReads);
    else
    	return -1;
}


int8_t HX711::waitingForReadyState(uint16_t timeInMilis){
	for(uint16_t i = 0; i < timeInMilis; ++i){
		HAL_Delay(1);
		if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 0 ) //ready
			return 1;
		else
			continue;
	}
	return 0;
}

void BlinkNTimesDuringXMilis(uint16_t blinkTimes, uint16_t timeInMilis){
	for(uint8_t i = 0; i < blinkTimes ; ++i){
		HAL_GPIO_TogglePin(BUILD_IN_LED_GPIO_Port, BUILD_IN_LED_Pin);
		HAL_Delay(timeInMilis / blinkTimes);
	}
	HAL_GPIO_WritePin(BUILD_IN_LED_GPIO_Port, BUILD_IN_LED_Pin, GPIO_PIN_SET);
}
