#include <Arduino.h>
#include <SPI.h>
#include "tasks.h"
#include "mainClass.h"
#include "config.h"


void setup() {
  #ifdef __DEBUG
  Serial.begin(115200);
  #endif

  TanWaControl * tc = new TanWaControl();  
  
  tc->sdQueue = xQueueCreate(SD_QUEUE_LENGTH, sizeof(char[SD_FRAME_SIZE]));
  tc->loraTxQueue = xQueueCreate(LORA_TX_QUEUE_LENGTH, sizeof(char[LORA_TX_FRAME_SIZE]));

  tc->spiMutex = xSemaphoreCreateMutex();

  vTaskDelay(25 / portTICK_PERIOD_MS);
  
  xTaskCreatePinnedToCore(loraTask, "LoRa task", 8096, (void*)tc, 3, &tc->loraTask, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(sdTask,   "SD task",   8096, (void*)tc, 3, &tc->sdTask,   APP_CPU_NUM);
  xTaskCreatePinnedToCore(dataTask, "Data task", 8096, (void*)tc, 3, &tc->dataTask, APP_CPU_NUM);

  if(tc->sdQueue == NULL){
    //error handling - queue create
  }

  if(tc->spiMutex == NULL){
    //error handling - mutex create
  }
  
  if(tc->loraTask == NULL || tc->sdTask == NULL || tc->dataTask == NULL){
    //error handling - task create
  }

  tc->changeState(State::IDLE);
  //vTaskDelete(NULL); //to usuwa loop
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}