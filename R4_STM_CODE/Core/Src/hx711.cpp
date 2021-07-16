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

int32_t HX711::ReadValue(){
	int32_t buffer = 0;
	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
    //wait for 0 on Dt_Pin
    uint8_t DT = 0;
    for(uint16_t i = 0; i < 10000; ++i){
    	DT = HAL_GPIO_ReadPin(Dt_gpio, Dt_pin);
    	if(DT == 0 )
    		break;
      	else if(DT == 1 && i > 9990)
    		return 0;
    	else
    		continue;
    }
    //main read loop
    __disable_irq();// Disable interrupts
    for (uint8_t i = 0; i < 24; ++i){
    	HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_SET);
        buffer = buffer << 1 ;
        buffer+= HAL_GPIO_ReadPin(Dt_gpio, Dt_pin);
        HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
    }
    __enable_irq(); // Enable interrupts back

    //wait for 1 on Dt_Pin
    for(uint16_t i = 0; i < 10000; ++i){
    	DT = HAL_GPIO_ReadPin(Dt_gpio, Dt_pin);
    	if(DT == 1)
       		break;
     	else if(DT == 1 && i > 9990)
        	return 0;
    	else
    		continue;
    }
    return (buffer<<7) / 128;
}

int32_t HX711::AverageValue(uint16_t sampleSize){
	int32_t sum = 0, highestValue = 0, lowestValue = 0;
	uint16_t succesfulReads = 0;
    for (uint16_t i = 0; i < sampleSize; ++i){
    	int32_t read = ReadValue();
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
