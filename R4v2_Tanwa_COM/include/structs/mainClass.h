#ifndef MAIN_CLASS
#define MAIN_CLASS

#include "FreeRTOS.h"
#include <SPI.h>

class TanWaControl{

  public:
  SPIClass mySPI;
  
  TanWaControl();
};

#endif