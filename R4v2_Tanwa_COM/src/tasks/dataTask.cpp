#include "../include/tasks/tasks.h"


// extern Hx711 rckWeight;
// extern Hx711 tankWeight;
// extern MCP23017 expander;
char data[SD_FRAME_SIZE] = {};
 PWRData pwrData;
//kod w tym tasku jest tylko do debugu 
void dataTask(void *arg){
  uint32_t abort_count = 0;
  int turnVar = 0;
  DataFrame dataFrame;
 
  expander.setPinMode(0,B,INPUT); //input for abort button

  //HX711
  rckWeight.begin();
  rckWeight.start(STABILIZNG_TIME, true); //start without tare
  rckWeight.setCalFactor(BIT_TO_GRAM_RATIO_RCK);
  // rckWeight.setTareOffset(OFFSET_RCK);
  rckWeight.setSamplesInUse(1);

  tankWeight.begin();
  tankWeight.start(STABILIZNG_TIME, true); //start without tare
  tankWeight.setCalFactor(BIT_TO_GRAM_RATIO_TANK);
  // tankWeight.setTareOffset(OFFSET_TANK);
  tankWeight.setSamplesInUse(1);
  while (tankWeight.getTareTimeoutFlag() && rckWeight.getTareTimeoutFlag())
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }



  // !!!//DEBUG
  //InternalI2C<PWRData, TxData> i2cCOM(&stm.i2c, COM_ADRESS);

  vTaskDelay(100 / portTICK_PERIOD_MS);
 
   
  while(1){
   
    xSemaphoreTake(stm.i2cMutex, pdTRUE);
    pwrCom.getData(&pwrData);
    xSemaphoreGive(stm.i2cMutex);


    xSemaphoreTake(stm.i2cMutex, pdTRUE);
    expander.setPinPullUp(2,B,turnVar);
    xSemaphoreGive(stm.i2cMutex);


    if(turnVar == 1)
      turnVar = 0;
    else
      turnVar = 1;

    if(tankWeight.update() == 1){
      dataFrame.tankWeight = tankWeight.getData();
      dataFrame.tankWeightRaw = (uint32_t) tankWeight.getRawData();
    }

    if(rckWeight.update() == 1){
      dataFrame.rocketWeight = rckWeight.getData();
      dataFrame.rocketWeightRaw = (uint32_t) rckWeight.getRawData();
    }

    dataFrame.vbat = voltageMeasure(VOLTAGE_MEASURE);
    // memcpy(dataFrame.motorState, pwrData.motorState, sizeof(uint8_t[5]));
    dataFrame.motorState_1 = pwrData.motorState[0];
    dataFrame.motorState_2 = pwrData.motorState[1];
    dataFrame.motorState_3 = pwrData.motorState[2];
    dataFrame.motorState_4 = pwrData.motorState[3];
    dataFrame.motorState_5 = pwrData.motorState[4];
  

    dataFrame.tanWaState = StateMachine::getCurrentState();
    
    dataFrame.igniterContinouity_1 = analogRead(IGN_TEST_CON_1) > 1000;
    dataFrame.igniterContinouity_2 = analogRead(IGN_TEST_CON_2) > 1000;


    createDataFrame(dataFrame, data);

    Serial.println(data);
    // xQueueSend(stm.loraTxQueue, (void*)data, 0);

    xQueueSend(stm.sdQueue, (void*)data, 0); 

      xSemaphoreTake(stm.i2cMutex, pdTRUE);
      if(expander.getPin(0,B)==0){// ABORT BUTTON
        abort_count++;
        if(abort_count>=3){
          expander.setPinPullUp(1,B,ON);
          StateMachine::changeStateRequest(States::ABORT);
        }
      }
      else{
        abort_count = 0;
        expander.setPinPullUp(1,B,OFF);
      }
       

      xSemaphoreGive(stm.i2cMutex);

      // Serial.println("RESeeeeeeeeeeeeeeeET");
      // vTaskDelay(5000 / portTICK_PERIOD_MS);
      // xSemaphoreTake(stm.i2cMutex, pdTRUE);
      // expander.setPinX(4,A,OUTPUT,ON);
      // xSemaphoreGive(stm.i2cMutex);
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

    Serial.print("PRESSURE bit: "); Serial.println(pwrData.adcValue[0]);

    dataFrame.pressureSensor = map(pwrData.adcValue[0],450, 4096, 0, 80);
    Serial.print("PRESSURE in bars: "); Serial.println(dataFrame.pressureSensor);
    Serial.print("TANWA VOLTAGE 4: "); Serial.println(voltageMeasure(VOLTAGE_MEASURE));
 

    Serial.print("TANK WEIGHT: "); Serial.println(dataFrame.tankWeight);
    Serial.print("ROCKET WEIGHT: "); Serial.println(dataFrame.rocketWeight);
    Serial.print("continuity 1 "); Serial.println(dataFrame.igniterContinouity_1);
    Serial.print("continuity 2 "); Serial.println(dataFrame.igniterContinouity_2);


  
    esp_now_send(adressObc, (uint8_t*) &dataFrame, sizeof(DataFrame));
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}