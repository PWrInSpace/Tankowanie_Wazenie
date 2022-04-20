//communication core
#include "tasks.h"

void loraTask(void *arg){
  char loraTx[LORA_TX_FRAME_SIZE] = {};

  TanWaControl * tc = static_cast<TanWaControl*>(arg);
  
  vTaskDelay(25 / portTICK_PERIOD_MS);
  
  //xSemaphoreTake(tc->spiMutex, pdTRUE);
  xSemaphoreTake(mutex, portMAX_DELAY);

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

  //xSemaphoreGive(tc->spiMutex);
  xSemaphoreGive(mutex);

  while(1){
    //DEBUGL("LORA TASK");
    //xSemaphoreTake(tc->spiMutex, portMAX_DELAY);
    xSemaphoreTake(mutex, portMAX_DELAY);


    if(LoRa.parsePacket() != 0){
      DEBUGL("LORA PARSE");
    }
    if (LoRa.available()) {

      String rxStr = LoRa.readString();
      DEBUG("ODEBRANO:");
      DEBUGL(rxStr); // DEBUG

    }

    //xSemaphoreGive(tc->spiMutex);
    xSemaphoreGive(mutex);

    if(xQueueReceive(tc->loraTxQueue, (void*)&loraTx, 0) == pdTRUE){
      xSemaphoreTake(mutex, portMAX_DELAY);

      //xSemaphoreTake(tc->spiMutex, portMAX_DELAY);
        
      if(LoRa.beginPacket() == 0){
        Serial.println("LORA is transmitnig");
      }
      LoRa.write((uint8_t*) loraTx, strlen(loraTx));
      if(LoRa.endPacket() != 1){
        Serial.println("End packet error!");
      }
      DEBUG("WYSLANO:");
      DEBUGL(loraTx); // DEBUG

      xSemaphoreGive(mutex);
      //xSemaphoreGive(tc->spiMutex);
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void weighingTask(void* arg){



  
}