#include <hx711.hh>

HX711::HX711(GPIO_TypeDef* _Dt_gpio, uint16_t _Dt_pin,
			 GPIO_TypeDef* _Sck_gpio, uint16_t _Sck_pin)
{
	Dt_gpio = _Dt_gpio;
	Dt_pin = _Dt_pin;
	Sck_gpio = _Sck_gpio;
	Sck_pin = _Sck_pin;
	BitsToGramRatio = 1;
	OffsetInGrams = 0;
}

int32_t HX711::getWeigthInGramsWithOffset(uint16_t times){
	return AverageValue(times) / BitsToGramRatio + OffsetInGrams;
}

int32_t HX711::getOffsetInGrams() const{
	return OffsetInGrams;
}

int32_t HX711::getBitsToGramRatio() const{
	return BitsToGramRatio;
}

void HX711::addToOffsetInGrams(int32_t OffsetInGramsDif){
	OffsetInGrams += OffsetInGramsDif;
}

void HX711::setBitsToGramRatio(int32_t newBitsToGramRatio){
	BitsToGramRatio = newBitsToGramRatio;
}

void HX711::initialCalibration(uint32_t testLoadInGrams){
	if (testWeightInGrams == 0)
		return;
	int32_t initialWeight = AverageValue();
	//HAL_Delay(10000); //put testWeight on load cell //odkomentowaxc pozniej
	int32_t weightWithLoad = AverageValue();
	BitsToGramRatio = (weightWithLoad - initialWeight) / testLoadInGrams;
	OffsetInGrams = testLoadInGrams - weightWithLoad / BitsToGramRatio;
}

int32_t HX711::ReadValue(){
    int32_t buffer=0;

    HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
    //wait for 0 on Dt_Pin
//    for(uint16_t i = 0; i < 1000; ++i){
//    	if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 0 )
//    		break;
//    	else if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 1 && i > 990)
//    		return 0;
//    	else
//    		continue;
//    }

    while(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 1 );
    //read weight
    for (uint8_t i = 0; i < 24; ++i){
    	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
        buffer = buffer << 1 ;
        buffer+= HAL_GPIO_ReadPin(Dt_gpio, Dt_pin);
        HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
    }

/* if gain is not 128: (gain(channel B)==32):gain=1; (gain==64):gain=2; */
//    for (uint8_t i=0; i < gain; i++){xbee.c
//    	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
//    	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
//    }

    //wait for 1 on Dt_Pin
    for(uint16_t i = 0; i < 1000; ++i){
    	if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 1)
       		break;
    	else
    		continue;
    }
    return (buffer<<7)/128;
}

int32_t HX711::AverageValue(uint16_t times){
	int32_t sum = 0;
	uint16_t succesfulReads = 0;
    for (uint16_t i = 0; i < times; ++i){
    	int32_t read = ReadValue();
    	if(read !=0){
    		sum += read;
    		++succesfulReads;
    	}
    }
    if(succesfulReads != 0)
    	return sum / succesfulReads;
    else
    	return 0;
}
