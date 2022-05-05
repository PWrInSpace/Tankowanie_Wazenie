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
  

  // !!!//DEBUG
  InternalI2C<PWRData, TxData> i2cCOM(&stm.i2c, COM_ADRESS);
  PWRData pwrData;

  vTaskDelay(100 / portTICK_PERIOD_MS);
  
  while(1){
    
    snprintf(data, SD_FRAME_SIZE, "TEST;%lu\n", millis());
    strcpy(lora, data);
    xQueueSend(stm.loraTxQueue, (void*)lora, 0);

    strcpy(sd, data);
    xQueueSend(stm.sdQueue, (void*)sd, 0);
    
    //DEBUG(data);
    
    i2cCOM.getData(&pwrData);
    Serial.println("\n\n\nCOM DATA:");
    Serial.print("BLINK: "); Serial.println(pwrData.tick);
    Serial.print("LAST COMMAND: "); Serial.println(pwrData.lastDoneCommandNum);
    Serial.print("MOTOR STATE 0: "); Serial.println(pwrData.motorState[0]);
    Serial.print("MOTOR STATE 1: "); Serial.println(pwrData.motorState[1]);
    Serial.print("MOTOR STATE 2: "); Serial.println(pwrData.motorState[2]);
    Serial.print("MOTOR STATE 3: "); Serial.println(pwrData.motorState[3]);
    Serial.print("MOTOR STATE 4: "); Serial.println(pwrData.motorState[4]);
    Serial.print("ADC VALUE 0: "); Serial.println(pwrData.adcValue[0]);
    Serial.print("ADC VALUE 1: "); Serial.println(pwrData.adcValue[1]);
    Serial.print("ADC VALUE 2: "); Serial.println(pwrData.adcValue[2]);
    Serial.print("ADC VALUE 3: "); Serial.println(pwrData.adcValue[3]);
    Serial.print("ADC VALUE 4: "); Serial.println(pwrData.adcValue[4]);
    Serial.print("ADC VALUE 5: "); Serial.println(pwrData.adcValue[5]);
    Serial.print("ADC VALUE 6: "); Serial.println(pwrData.adcValue[6]);
    Serial.print("ADC VALUE 7: "); Serial.println(pwrData.adcValue[7]);

    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}