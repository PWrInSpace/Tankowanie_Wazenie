#include "mainClass.h"

TanWaControl::TanWaControl():
  state(State::INIT),
  loraTask(NULL),
  sdTask(NULL),
  dataTask(NULL),
  sdQueue(NULL),
  loraTxQueue(NULL),
  spiMutex(NULL),
  mySPI(SPIClass(VSPI))
{}


State TanWaControl::getState() const{
  return state;
}

void TanWaControl::changeState(State _state){
  state = _state;
}