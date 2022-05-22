#include "../include/tasks/tasks.h"

//kod w tym tasku jest tylko do debugu 
void dataTask(void *arg){
  char data[SD_FRAME_SIZE] = {};
  DataFrame dataFrame;

  //HX711
  HX711_ADC rckWeight(HX1_SDA, HX1_SCL);
  HX711_ADC tankWeight(HX2_SDA, HX2_SCL);

  rckWeight.begin();
  rckWeight.start(STABILIZNG_TIME, false); //start without tare
  rckWeight.setCalFactor(BIT_TO_GRAM_RATIO_RCK);
  rckWeight.setTareOffset(OFFSET_RCK);
  rckWeight.setSamplesInUse(16);
  rckWeight.setGain(64);

  tankWeight.begin();
  tankWeight.start(STABILIZNG_TIME, false); //start without tare
  tankWeight.setCalFactor(BIT_TO_GRAM_RATIO_TANK);
  tankWeight.setTareOffset(OFFSET_TANK);
  tankWeight.setSamplesInUse(16);
  tankWeight.setGain(64);
  //calibration after rest or on eg continuity with the use of CUSTOM function
  // rckWeight.CustomCalibration(float known_mass, int delay_ms);
  // tankWeight.CustomCalibration(float kown_mass, int delay_ms);
  //

  // !!!//DEBUG
  InternalI2C<PWRData, TxData> i2cCOM(&stm.i2c, COM_ADRESS);
  PWRData pwrData;

  vTaskDelay(100 / portTICK_PERIOD_MS);
 
  
  
  while(1){

    xSemaphoreTake(stm.i2cMutex, pdTRUE);
    i2cCOM.getData(&pwrData);
    xSemaphoreGive(stm.i2cMutex);
    
    dataFrame.vbat = pwrData.adcValue[4];
    if(tankWeight.update() == 1){
      dataFrame.tankWeight = tankWeight.getData();
      dataFrame.tankWeightRaw = (uint32_t) tankWeight.getRawData();
    }

    if(rckWeight.update() == 1){
      dataFrame.rocketWeight = rckWeight.getData();
      dataFrame.rocketWeightRaw = (uint32_t) rckWeight.getRawData();
    }

    createDataFrame(dataFrame, data);

    Serial.println(data);
    xQueueSend(stm.loraTxQueue, (void*)data, 0);

    xQueueSend(stm.sdQueue, (void*)data, 0);
    
    //DEBUG(data);
    // xSemaphoreTake(stm.i2cMutex, pdTRUE);
    // i2cCOM.getData(&pwrData);
    // xSemaphoreGive(stm.i2cMutex);
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
    Serial.print("TANWA VOLTAGE 4: "); Serial.println(pwrData.adcValue[4]);
    Serial.print("ADC VALUE 5: "); Serial.println(pwrData.adcValue[5]);
    Serial.print("ADC VALUE 6: "); Serial.println(pwrData.adcValue[6]);
    Serial.print("ADC VALUE 7: "); Serial.println(pwrData.adcValue[7]);
  

    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}