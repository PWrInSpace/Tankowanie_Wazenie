#ifndef PINOUT_H
#define PINOUT_H

#include "FreeRTOS.h"
#include "Arduino.h"

// igniters
#define IGN_TEST_CON_1 34
#define IGN_TEST_CON_2 35
#define FIRE1 27
#define FIRE2 26
#define ARM_PIN 32

#define VOLTAGE_MEASURE 36

#define RESET_COMMAND 99

//LORA
#define LORA_D0 39
#define LORA_CS 5
#define LORA_RS 4

//BUZZER & SPEAKER
#define BUZZER 33
#define SPEAKER 25

//HX711
#define HX1_SDA 15
#define HX1_SCL 12
#define HX2_SDA 14
#define HX2_SCL 12

//RESET
#define RST 13

//SPI
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

//I2C   
#define I2C_SCL 22
#define I2C_SDA 21

//SD CHIP SELECT
#define SD_CS 17

#define TX 1
#define RX 3

//CAMERA 
#define RUNCAM 16

#define BOOT 0 //????????


void beepBoop(int delay, int times);
float voltageMeasure(uint8_t batteryPin, const float &reverseDividerVal);
void pinInit();

#endif