//application core
#include "tasks.h"

void sdTask(void *arg){
  TanWaControl * tc = static_cast<TanWaControl*>(arg);
  char data[SD_FRAME_SIZE] = {};
  String dataPath = dataFileName;
  uint8_t sd_i = 0;
  
  vTaskDelay(100 / portTICK_RATE_MS);

  SDCard mySD(tc->mySPI, SD_CS);

  //xSemaphoreTake(tc->spiMutex, pdTRUE);
  xSemaphoreTake(mutex, portMAX_DELAY);

  while(!mySD.init()){
    Serial.println("SD INIT ERROR!");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  while(mySD.fileExists(dataPath + String(sd_i) + ".txt")){
    sd_i++;
  }
  dataPath = dataPath + String(sd_i) + ".txt";

  //xSemaphoreGive(tc->spiMutex);
  xSemaphoreGive(mutex);

  while(1){
    if(xQueueReceive(tc->sdQueue, (void*)&data, 0) == pdTRUE){
      //xSemaphoreTake(tc->spiMutex, portMAX_DELAY);
      xSemaphoreTake(mutex, portMAX_DELAY);  

      if(!mySD.write(dataPath, data)){
        //SD_WRITE_ERROR
      }
        
      DEBUG("ZAPIS NA SD");
      //xSemaphoreGive(tc->spiMutex);
      xSemaphoreGive(mutex);
    }
   
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}


//kod w tym tasku jest tylko do debugu 
void dataTask(void *arg){
  char sd[SD_FRAME_SIZE] = {};
  char lora[LORA_TX_FRAME_SIZE] = {};
  char data[SD_FRAME_SIZE] = {};

  TanWaControl *tc = static_cast<TanWaControl*>(arg);

  
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  
  while(1){
    
    snprintf(data, SD_FRAME_SIZE, "%lu;%d\n", millis(), tc->getState());
    strcpy(lora, data);
    xQueueSend(tc->loraTxQueue, (void*)lora, 0);

    strcpy(sd, data);
    xQueueSend(tc->sdQueue, (void*)sd, 0);
    
    DEBUG(data);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}