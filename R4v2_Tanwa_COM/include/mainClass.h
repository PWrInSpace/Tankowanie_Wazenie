#ifndef MAIN_CLASS
#define MAIN_CLASS

#include "FreeRTOS.h"
#include <SPI.h>

enum State{
  INIT = 0,
  IDLE,
  ARMED,
  FUELING,
  RDY_TO_LAUNCH,
  COUNTDOWN,
  FLIGHT,
  ABORT
};


class TanWaControl{
  State state;

  public:
  TaskHandle_t loraTask;
  TaskHandle_t sdTask;
  TaskHandle_t dataTask;
  //... inne taski
  QueueHandle_t sdQueue;
  QueueHandle_t loraTxQueue;

  SemaphoreHandle_t spiMutex;

  SPIClass mySPI;

  TanWaControl();
  State getState() const;
  void changeState(State _state);
};

#endif