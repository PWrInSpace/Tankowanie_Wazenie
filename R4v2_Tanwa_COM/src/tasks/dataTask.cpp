#include "../include/tasks/tasks.h"

//kod w tym tasku jest tylko do debugu 
void dataTask(void *arg){
  char sd[SD_FRAME_SIZE] = {};
  char lora[LORA_TX_FRAME_SIZE] = {};
  char data[SD_FRAME_SIZE] = {};

  HX711_ADC rckWeight(HX1_SDA, HX1_SCL);
  HX711_ADC tankWeight(HX2_SDA, HX2_SCL);


  //TanWaControl *tc = static_cast<TanWaControl*>(arg);

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
  
  vTaskDelay(100 / portTICK_PERIOD_MS);
  
  while(1){
    
    snprintf(data, SD_FRAME_SIZE, "TEST;%lu\n", millis());
    strcpy(lora, data);
    xQueueSend(stm.loraTxQueue, (void*)lora, 0);

    strcpy(sd, data);
    xQueueSend(stm.sdQueue, (void*)sd, 0);
    
    DEBUG(data);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}