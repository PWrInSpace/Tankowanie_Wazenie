#ifndef DATA_HH
#define DATA_HH

#include "../config/config.h"
#include "../structs/stateMachine.h"

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

struct Options{
  uint16_t loraFreqMHz;
  uint32_t bitToGramsRatioTank;
  uint32_t bitToGramsRatioRocket;

  Options():
    loraFreqMHz(LORA_FREQ_MHZ),
    bitToGramsRatioTank(BIT_TO_GRAM_RATIO_TANK),
    bitToGramsRatioRocket(BIT_TO_GRAM_RATIO_RCK)
  {}
};

struct DataFrame{
  States tanWaState;
  bool tankHeating : 1;
  bool abortButton : 1;
  bool armButton : 1;
  bool iginiterContinouity[2];
  float vbat;
  ValveState motorState[5];
  float rocketWeight;
  float butlaWeight; 
  uint32_t rocketWeightRaw;
  uint32_t butlaWeightRaw;
  float thermocouple[3];
};

#endif