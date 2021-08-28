#include <hx711.hh>

HX711::HX711(GPIO_TypeDef* _Dt_gpio, uint16_t _Dt_pin,
			 GPIO_TypeDef* _Sck_gpio, uint16_t _Sck_pin):
	Dt_gpio(_Dt_gpio), Dt_pin(_Dt_pin), Sck_gpio(_Sck_gpio), Sck_pin(_Sck_pin), BitsToGramRatio(0)
{}

int32_t HX711::getWeigthInGramsWithOffset(uint16_t times){
	int32_t average = AverageValue(times);
	if(BitsToGramRatio != 0){
		return (average + OffsetInBits) / BitsToGramRatio;
	}
	else //not calibration
		return average;
}

int32_t HX711::getOffsetInBits() const{
	return OffsetInBits;
}

int32_t HX711::getBitsToGramRatio() const{
	return BitsToGramRatio;
}

void HX711::tare(){
	OffsetInBits = -AverageValue();
}

void HX711::initialCalibration(int32_t testLoadInGrams, uint16_t calibrationTime){
	if (testLoadInGrams == 0)
		return;
	//leave load cell empty
	int32_t initialWeight = AverageValue();
	OffsetInBits = -initialWeight;
	//put testWeight on load cell
	HAL_Delay(calibrationTime);
	int32_t weightWithLoad = AverageValue();
	BitsToGramRatio = (weightWithLoad - initialWeight) / testLoadInGrams;
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

int32_t HX711::ReadValue(){
	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
	int32_t buffer = 0;

	for(uint16_t i = 0; i < 10000; ++i){
		if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 0 )
			break;
		else if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 1 && i > 9990)
			return 0;
		else
			continue;
	}

	for (uint8_t i = 0; i < 24; ++i){
		HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
		buffer = buffer << 1 ;
		buffer+=HAL_GPIO_ReadPin(Dt_gpio, Dt_pin);
		HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);

    for(uint16_t i = 0; i < 10000; ++i){
		if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 1)
			break;
		else
			continue;
    }
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
