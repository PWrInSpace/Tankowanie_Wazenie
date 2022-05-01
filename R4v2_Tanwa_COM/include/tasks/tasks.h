#ifndef TASKS_HH
#define TASKS_HH

#include "../structs/SoftToolsManagment.h"
#include "../config/pinout.h"
#include "LoRa.h"
#include "../config/config.h"
#include "../components/SDcard.h"
#include <HX711_ADC.h>
#include "../structs/stateMachine.h"

extern SoftwareToolsManagment stm;

//pro_cpu
void loraTask(void *arg);


//app_cpu
void sdTask(void *arg);
void dataTask(void *arg);
void stateTask(void *arg);


#endif