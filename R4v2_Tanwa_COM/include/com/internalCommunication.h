#ifndef INTERNAL_COMMUNICATION_HH
#define INTERNAL_COMMUNICATION_HH

#include "../include/structs/commStructs.h"
#include <Wire.h>


// #define IODIRA      0x00   
// #define IODIRB      0x01 
// #define IOCONA      0x0A   
// #define IOCONB      0x0B  
// #define INTCAPA     0x10  
// #define INTCAPB     0x11  
// #define INTCONA     0x08  
// #define INTCONB     0x09  
// #define INTFA       0x0E    
// #define INTFB       0x0F
// #define GPINTENA    0x04 
// #define GPINTENB    0x05
// #define DEFVALA     0x06  
// #define DEFVALB     0x07
// #define IPOLA       0x02   
// #define GPIOA       0x12    
// #define GPIOB       0x13
// #define INTPOL      1    
// #define INTODR      2
// #define MIRROR      6    
// #define GPPUA       0x0C  
// #define GPPUB       0x0D
// #define SPI_READ    0x01

// enum MCP_PORT2 {A2, B2};
// enum STATE2 {OFF2, ON2};


// int I2C_Address = 0x20;
// int SPI_Address;
// int resetPin;
// int csPin;
// bool useSPI;
// uint8_t ioDirA, ioDirB;
// uint8_t gpioA, gpioB;
// uint8_t gpIntEnA, gpIntEnB;
// uint8_t ioConA, ioConB;
// uint8_t intConA, intConB;
// uint8_t defValA, defValB;
// uint8_t gppuA, gppuB;


template <typename rxType, typename txType>
class InternalI2C{  
  TwoWire *i2c;
  uint8_t address;
  
  public:
  InternalI2C(TwoWire* _wire, uint8_t _adress);

  bool sendCommand(txType *_data);
  bool sendCommandMotor(uint8_t command_valve, uint8_t command_state, uint16_t commandValue = 0);
  bool getData(rxType* _data);
  bool sendExpander(uint8_t pin, char port, uint8_t ioDir, uint8_t state);
};

template class InternalI2C<PWRData, TxData>;

#endif