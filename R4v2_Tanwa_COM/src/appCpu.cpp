//application core
#include "tasks.h"
#include "data.h"

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
  DataStruct dataStruct;
  HX711_ADC rckWeight(HX1_SDA, HX1_SCL);
  HX711_ADC tankWeight(HX2_SDA, HX2_SCL);


  TanWaControl *tc = static_cast<TanWaControl*>(arg);

  rckWeight.begin();
  rckWeight.start(STABILIZNG_TIME, false); //start without tare
  rckWeight.setCalFactor(BIT_TO_GRAM_RATIO_RCK);
  rckWeight.setTareOffset(OFFSET_RCK);
  rckWeight.setSamplesInUse(20);
  rckWeight.setGain(64);

  tankWeight.begin();
  tankWeight.start(STABILIZNG_TIME, false); //start without tare
  tankWeight.setCalFactor(BIT_TO_GRAM_RATIO_TANK);
  tankWeight.setTareOffset(OFFSET_TANK);
  tankWeight.setSamplesInUse(20);
  tankWeight.setGain(64);

  // TODO na komende nowa kalibracja
  //rckWeight.getNewCalibration();
  //tankWeight.getNewCalibration();

  vTaskDelay(5000 / portTICK_PERIOD_MS);
  
  while(1){
    
    snprintf(data, SD_FRAME_SIZE, "%lu;%d\n", millis(), tc->getState());
    strcpy(lora, data);
    xQueueSend(tc->loraTxQueue, (void*)lora, 0);

    strcpy(sd, data);
    xQueueSend(tc->sdQueue, (void*)sd, 0);
    
    DEBUG(data);
   
    dataStruct.rckWeight = rckWeight.getData();
    dataStruct.tankWeight = tankWeight.getData();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
  }
}