#include <hx711.hh>

HX711::HX711(GPIO_TypeDef* _Dt_gpio, uint16_t _Dt_pin, GPIO_TypeDef* _Sck_gpio, uint16_t _Sck_pin){
	Dt_gpio = _Dt_gpio;
	Dt_pin = _Dt_pin;
	Sck_gpio = _Sck_gpio;
	Sck_pin = _Sck_pin;
	calibrationFactor = 1;
	offset = -Average_Value(5);
}

int32_t HX711::getWeigthPlusOffsetAfterConversion(uint16_t times){
	return Average_Value(times) / calibrationFactor + offset;
}

int32_t HX711::getOffset() const{
	return offset;
}

int32_t HX711::getCalibrationFactor() const{
	return calibrationFactor;
}

void HX711::addToOffset(int32_t offsetDif){
	offset += offsetDif;
}

void HX711::setCalibrationFactor(int32_t newCalibrationFactor){
	calibrationFactor = newCalibrationFactor;
}


int32_t HX711::Read_Value(){
    int32_t buffer=0;

    HAL_GPIO_WritePin(Sck_gpio, Sck_pin, GPIO_PIN_RESET);
    //wait for 0 on Dt_Pin
    for(uint8_t i = 0; i < 1000; ++i){
    	if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 0 )
    		break;
    	else if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 1 && i > 990)
    		return 0;
    	else
    		continue;
    }
    //read weight
    for (uint8_t i = 0; i < 25; ++i){
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
    for(uint8_t i = 0; i < 1000; ++i){
    	if(HAL_GPIO_ReadPin(Dt_gpio, Dt_pin) == 1)
       		break;
    	else
    		continue;
    }
    return (buffer<<7)/128;
}

int32_t HX711::Average_Value(uint16_t times){
	int32_t sum = 0;
	uint16_t succesfulReads = 0;
    for (uint16_t i = 0; i < times; ++i){
    	int32_t read = Read_Value();
    	if(read > 0){
    		sum += read;
    		++succesfulReads;
    	}
    }
    if(succesfulReads != 0)
    	return sum / succesfulReads;
    else
    	return 0;
}

