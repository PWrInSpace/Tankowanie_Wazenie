//communication core
#include "tasks.h"

void loraTask(void *arg){
  char loraTx[LORA_TX_FRAME_SIZE] = {};

  TanWaControl * tc = static_cast<TanWaControl*>(arg);
  
  vTaskDelay(25 / portTICK_PERIOD_MS);
  
  xSemaphoreTake(tc->spiMutex, pdTRUE);

  LoRa.setSPI(tc->mySPI);
  LoRa.setPins(LORA_CS, LORA_RS, LORA_D0);
  
  while(LoRa.begin(868 * 1E6) == 0){ //DEBUG
    Serial.println("LORA begin error!");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  LoRa.setSignalBandwidth(250E3);
  LoRa.disableCrc();
  LoRa.setSpreadingFactor(7);
  LoRa.setTxPower(14);
  LoRa.setTimeout(10);

  xSemaphoreGive(tc->spiMutex);

  while(1){
    DEBUG("LORA TASK");
    xSemaphoreTake(tc->spiMutex, portMAX_DELAY);

    if(LoRa.parsePacket() != 0){
      DEBUG("LORA PARSE");
    }
    if (LoRa.available()) {

      String rxStr = LoRa.readString();
      DEBUG("ODEBRANO:");
      DEBUG(rxStr); // DEBUG

    }

    xSemaphoreGive(tc->spiMutex);

    if(xQueueReceive(tc->loraTxQueue, (void*)&loraTx, 0) == pdTRUE){
      xSemaphoreTake(tc->spiMutex, portMAX_DELAY);
        
      if(LoRa.beginPacket() == 0){
        Serial.println("LORA is transmitnig");
      }
      LoRa.write((uint8_t*) loraTx, strlen(loraTx));
      if(LoRa.endPacket() != 1){
        Serial.println("End packet error!");
      }
      DEBUG("WYSLANO:");
      DEBUG(loraTx); // DEBUG

      xSemaphoreGive(tc->spiMutex);
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}