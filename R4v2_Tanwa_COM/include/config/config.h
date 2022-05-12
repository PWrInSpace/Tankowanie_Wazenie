#ifndef TANWA_CONFIG_H
#define TANWA_CONFIG_H

#include <Arduino.h>

//#define __DEBUG

#ifdef __DEBUG
  #define DEBUGL(x) Serial.println(x)
  #define DEBUG(x) Serial.print(x)
#else
  #define DEBUGL(x) {}
  #define DEBUG(x) {}
#endif

//RTOS QUEUE
#define SD_FRAME_SIZE 256
#define SD_QUEUE_LENGTH 10
#define LORA_TX_FRAME_SIZE 256
#define LORA_TX_QUEUE_LENGTH 10
#define ESP_NOW_QUEUE_LENGTH 10
#define LORA_RX_QUEUE_LENGTH 256
#define LORA_RX_FRAME_SIZE 10
#define DATA_PREFIX "R4T;"

//SD FILE PATH
static String dataFileName= "/data_";

//I2C COM COMMAND
#define OPEN_FILL_VALVE 0x01
#define CLOSE_FILL_VALVE 0x02
#define OPEN_DEPR_VALVE 0x03
#define CLOSE_DEPR_VALVE 0x04


#define LORA_FREQ_MHZ 868

//WEIGHTING
#define BIT_TO_GRAM_RATIO_RCK 1
#define BIT_TO_GRAM_RATIO_TANK 1
#define OFFSET_RCK 1
#define OFFSET_TANK 1
#define STABILIZNG_TIME 4000

#define COM_ADRESS 0x1A

#endif