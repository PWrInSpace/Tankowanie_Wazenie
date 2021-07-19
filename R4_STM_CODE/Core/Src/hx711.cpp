#include <hx711.hh>

HX711::HX711(GPIO_TypeDef* _Dt_gpio, uint16_t _Dt_pin,
			 GPIO_TypeDef* _Sck_gpio, uint16_t _Sck_pin):
	Dt_gpio(_Dt_gpio), Dt_pin(_Dt_pin), Sck_gpio(_Sck_gpio), Sck_pin(_Sck_pin), BitsToGramRatio(1)
{
	tare();
}

int32_t HX711::getWeigthInGramsWithOffset(uint16_t times){
	if(BitsToGramRatio != 0)
		return (AverageValue(times) + OffsetInBits) / BitsToGramRatio;
	else
		return 0;
}

int32_t HX711::getBitsToGramRatio() const{
	return BitsToGramRatio;
}

void HX711::tare(){
	OffsetInBits = -AverageValue();
}

void HX711::initialCalibration(uint32_t testLoadInGrams){
	if (testLoadInGrams == 0)
		return;
	//leave load cell empty
	int32_t initialWeight = AverageValue();
	//put testWeight on load cell
	HAL_Delay(10000);
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
	 int32_t buffer=0;

	    HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);

	    while (HAL_GPIO_ReadPin(Dt_gpio, Dt_pin)==1)
	    	;

	    for (uint8_t i = 0; i < 25; i++){
	    	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
	        buffer = buffer << 1 ;
	        buffer+=HAL_GPIO_ReadPin(Dt_gpio, Dt_pin);
	        HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
	    }

	/* if gain is not 128: (gain(channel B)==32):gain=1; (gain==64):gain=2; */
	//    for (uint8_t i=0; i < gain; i++){
	//    	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
	//    	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
	//    }

	    while (HAL_GPIO_ReadPin(Dt_gpio, Dt_pin)==0)
	    	;

	    return (buffer<<7)/128;
}

int32_t HX711::AverageValue(uint16_t sampleSize){
	int32_t sum = 0, read = 0, highestValue = -99999999, lowestValue = 99999999;
	uint16_t succesfulReads = 0;
    for (uint16_t i = 0; i < sampleSize; ++i){
    	read = ReadValue();
    	if(read != 0){
    	    if(read > highestValue){
    	    	highestValue = read;
    	    }
    	    else if(read < lowestValue){
    	    	lowestValue = read;
    		}
    	    sum += read;
    		++succesfulReads;
    	}
    }
    //cut lowest and highest reads
    sum = sum - (lowestValue + highestValue);
    succesfulReads -= 2;
    if(succesfulReads > 0)
    	return sum / (succesfulReads);
    else
    	return -1;
}
