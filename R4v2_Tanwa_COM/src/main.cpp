#include <Arduino.h>
#include <SPI.h>
#include "../include/tasks/tasks.h"
#include "../include/config/config.h"
#include "../include/structs/SoftToolsManagment.h"
#include "../include/structs/commStructs.h"
#include "../include/com/now.h"
#include <esp_wifi.h>
#include <MCP23017.h>

SoftwareToolsManagment stm;
InternalI2C<PWRData, TxData> pwrCom(&stm.i2c, COM_ADRESS);
// HX711
Hx711 rckWeight(HX1_SDA, HX1_SCL);
Hx711 tankWeight(HX2_SDA, HX2_SCL);

MCP23017 expander = MCP23017(&stm.i2c,MCP_ADDRESS,RST);

void setup() {

  Serial.begin(115200);
  pinInit();
  // digitalWrite(ARM_PIN,1);
  // delay(1000);
  // digitalWrite(FIRE1,1);
  // digitalWrite(FIRE2,1);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_mac(ESP_IF_WIFI_STA, adressTanwa);

  ledcSetup(0,2000,8);// PWM FOR BUZZER
  ledcAttachPin(SPEAKER, 0);
  
  stm.i2c.begin(I2C_SDA, I2C_SCL, 100E3);
  stm.spi.begin();

  if(!expander.Init())
    Serial.println("Not connected!");
  else{
 
    expander.softReset(); //WARNING - EXPANDER ON SECOND PCB NEEDED THIS!!!!!! CHECK BEHAVIOUR FOR 1ST ONE

    Serial.println("CONNECTED");
    expander.setPinPullUp(5,A,OFF); //all termopary down
    expander.setPinPullUp(6,A,OFF);
    expander.setPinPullUp(7,A,OFF);

    expander.setPinX(4,A,OUTPUT,ON); // STM RST OFF
    expander.setPinX(0,B,INPUT,ON); //input for abort button

    expander.setPinPullUp(1,B,OFF);// all leds off
    expander.setPinPullUp(2,B,OFF);
    expander.setPinPullUp(3,B,OFF);
    expander.setPinPullUp(4,B,OFF);
    expander.setPinPullUp(5,B,OFF);
    expander.setPinPullUp(6,B,OFF);
    expander.setPinPullUp(7,B,OFF);

  }
 
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
  xTaskCreatePinnedToCore(rxHandlingTask, "Rx handling task", 8096, NULL, 2, &stm.rxHandlingTask, APP_CPU_NUM);
  xTaskCreatePinnedToCore(sdTask,   "SD task",   8096, NULL, 3, &stm.sdTask,   APP_CPU_NUM);
  xTaskCreatePinnedToCore(dataTask, "Data task", 8096, NULL, 3, &stm.dataTask, APP_CPU_NUM);
  xTaskCreatePinnedToCore(stateTask, "State task", 8096, NULL, 10, &stm.stateTask, APP_CPU_NUM);
  xTaskCreatePinnedToCore(buzzerTask, "Buzzer task", 8096, NULL, 3, &stm.buzzerTask, APP_CPU_NUM);


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
  
  vTaskDelete(NULL);

}

void loop() {}

