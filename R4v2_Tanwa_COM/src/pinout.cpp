#include "pinout.h"
#include <Wire.h>

float voltageMeasure(uint8_t batteryPin)
{
    // reverseDividerVal = (R1 + R2) / R2 = 58/11
    return (analogRead(batteryPin) / 4095) * 3.3 * (58/11);//TODO sprawdizc rezystor
}


void beepBoop(int time, int howManyTimes){
    uint32_t delay = time / howManyTimes;
    for(int i=0; i < howManyTimes; ++i){
        digitalWrite(BUZZER, 1);
        vTaskDelay(delay / portTICK_PERIOD_MS);
        digitalWrite(BUZZER, 0);
        vTaskDelay(delay / portTICK_PERIOD_MS);
    }
}


void pinInit()
{
    //igniter
    pinMode(IGN_TEST_CON_1, INPUT); 
    pinMode(IGN_TEST_CON_2, INPUT);
    pinMode(ARM, OUTPUT);
    pinMode(FIRE1, OUTPUT);
    pinMode(FIRE2, OUTPUT);
    digitalWrite(ARM, LOW);
    digitalWrite(FIRE1, LOW);
    digitalWrite(FIRE2, LOW);

    //VOLTAGE
    pinMode(VOLTAGE_MEASURE, INPUT);

    //LORA


    //BUZZER & SPEAKER
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
    pinMode(SPEAKER, OUTPUT);
    digitalWrite(SPEAKER, LOW);

    //RESET
    pinMode(RST, INPUT);


    //I2C
    Wire.begin(I2C_SDA, I2C_SCL);


    //CAMERA
    pinMode(RUNCAM, OUTPUT);
    digitalWrite(BUZZER, HIGH);

    //
}