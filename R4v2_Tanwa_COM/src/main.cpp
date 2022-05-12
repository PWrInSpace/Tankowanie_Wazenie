#include <Arduino.h>
#include <SPI.h>
#include "../include/tasks/tasks.h"
#include "../include/config/config.h"
#include "../include/structs/SoftToolsManagment.h"
#include "../include/structs/commStructs.h"
#include "../include/com/now.h"

SoftwareToolsManagment stm;
InternalI2C<PWRData, TxData> pwrCom(&stm.i2c, COM_ADRESS);

void setup() {
  //#ifdef __DEBUG
  //Serial.begin(115200);
  //#endif
  Serial.begin(115200);

  stm.i2c.begin(I2C_SDA, I2C_SCL, 100E3);

  nowInit();
  nowAddPeer(adressObc, 0);

  stm.sdQueue = xQueueCreate(SD_QUEUE_LENGTH, sizeof(char[SD_FRAME_SIZE]));
  stm.loraTxQueue = xQueueCreate(LORA_TX_QUEUE_LENGTH, sizeof(char[LORA_TX_FRAME_SIZE]));
  stm.loraRxQueue = xQueueCreate(LORA_RX_QUEUE_LENGTH, sizeof(char[LORA_RX_FRAME_SIZE]));
  stm.espNowRxQueue = xQueueCreate(ESP_NOW_QUEUE_LENGTH, sizeof(TxData));

  stm.i2cMutex = xSemaphoreCreateMutex();
  stm.spiMutex = xSemaphoreCreateMutex();

  vTaskDelay(25 / portTICK_PERIOD_MS);
  
  xTaskCreatePinnedToCore(loraTask, "LoRa task", 8096, NULL, 3, &stm.loraTask, PRO_CPU_NUM);
  xTaskCreatePinnedToCore(rxHandlingTask, "Rx handling task", 8096, NULL, 2, &stm.rxHandlingTask, PRO_CPU_NUM);
  
  xTaskCreatePinnedToCore(sdTask,   "SD task",   8096, NULL, 3, &stm.sdTask,   APP_CPU_NUM);
  xTaskCreatePinnedToCore(dataTask, "Data task", 8096, NULL, 3, &stm.dataTask, APP_CPU_NUM);
  xTaskCreatePinnedToCore(stateTask, "State task", 8096, NULL, 10, &stm.stateTask, APP_CPU_NUM);

  if(stm.sdQueue == NULL || stm.loraTxQueue == NULL){
    ESP.restart();
  }

  if(stm.spiMutex == NULL || stm.i2cMutex == NULL){
    ESP.restart();
  }

  /*
  if(stm.loraTask == NULL || stm.sdTask == NULL || stm.dataTask == NULL){
   ESP.restart();
  }*/
  StateMachine::changeStateRequest(States::IDLE);
  //stm.changeState(State::IDLE);
  vTaskDelete(NULL); 
}

void loop() {}