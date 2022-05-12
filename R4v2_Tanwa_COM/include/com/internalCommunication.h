#ifndef INTERNAL_COMMUNICATION_HH
#define INTERNAL_COMMUNICATION_HH

#include "../include/structs/commStructs.h"
#include <Wire.h>

template <typename rxType, typename txType>
class InternalI2C{  
  TwoWire *i2c;
  uint8_t address;
  
  public:
  InternalI2C(TwoWire* _wire, uint8_t _adress);

  bool sendCommand(txType *_data);
  bool sendCommandMotor(uint8_t command, uint8_t commandValue);
  bool getData(rxType* _data);
};

template class InternalI2C<PWRData, TxData>;

#endif