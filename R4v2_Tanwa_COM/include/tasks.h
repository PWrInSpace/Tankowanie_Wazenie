#ifndef TASKS_HH
#define TASKS_HH

#include "FreeRTOS.h"
#include "config.h"
#include "mainClass.h"
#include "LoRa.h"
#include "pinout.h"
#include "SDcard.h"

extern SemaphoreHandle_t mutex;

//pro_cpu
void loraTask(void *arg);

//app_cpu
void sdTask(void *arg);
void dataTask(void *arg);

#endif