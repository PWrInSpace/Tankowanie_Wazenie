#ifndef HX711_HH
#define HX711_HH

#include "HX711_ADC.h"

class Hx711:public HX711_ADC{
   
   	
    public:
    Hx711(uint8_t dout, uint8_t sck):HX711_ADC(dout, sck){};
    float CustomCalibration(float known_mass, int delay_ms);
    float getRawData();
};



#endif
