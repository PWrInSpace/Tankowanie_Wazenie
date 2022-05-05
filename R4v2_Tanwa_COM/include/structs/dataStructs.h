#ifndef data_H
#define data_H
#include <HX711_ADC.h>

enum ValveState {
  ValveStateClose = 0, 
  ValveStateOpen = 1, 
  ValveStateIDK = 2 , 
  ValveStateVent = 3, 
  ValveStateAttemptToOpen = 4, 
  ValveStateAttemptToClose = 5
};


struct TxData{
    uint8_t command;
    uint16_t commandValue;
};


struct PWRData{
  bool tick = {};
  uint8_t lastDoneCommandNum = {};
  ValveState motorState[5] = {};
  float adcValue[8] = {};
};

#endif