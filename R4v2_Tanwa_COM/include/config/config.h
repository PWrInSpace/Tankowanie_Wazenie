#ifndef TANWA_CONFIG_H
#define TANWA_CONFIG_H

#include <Arduino.h>

#define __DEBUG

#ifdef __DEBUG
  #define DEBUGL(x) Serial.println(x)
  #define DEBUG(x) Serial.print(x)
#else
  #define DEBUGL(x) {}
  #define DEBUG(x) {}
#endif

#define SD_FRAME_SIZE 256
#define SD_QUEUE_LENGTH 10
#define LORA_TX_FRAME_SIZE 256
#define LORA_TX_QUEUE_LENGTH 10

static String dataFileName= "/data_";

//weighing
#define BIT_TO_GRAM_RATIO_RCK 1
#define BIT_TO_GRAM_RATIO_TANK 1
#define OFFSET_RCK 1
#define OFFSET_TANK 1
#define STABILIZNG_TIME 4000

#endif