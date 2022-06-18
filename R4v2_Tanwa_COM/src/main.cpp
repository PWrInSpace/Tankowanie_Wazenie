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
// char portA = 'A';
// char portB = 'B';

MCP23017 expander = MCP23017(&stm.i2c,MCP_ADDRESS,RST);

void setup() {
  // //#ifdef __DEBUG
  // // Serial.begin(115200);
  // //#endif
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_mac(ESP_IF_WIFI_STA, adressTanwa);
  
  stm.i2c.begin(I2C_SDA, I2C_SCL, 100E3);
  stm.spi.begin();


  if(!expander.Init())
  Serial.println("Not connected!");
  else
    Serial.println("CONNECTED");

  expander.setPinPullUp(5,A,OFF); //all termopary down
  expander.setPinPullUp(6,A,OFF);
  expander.setPinPullUp(7,A,OFF);

  expander.setPinPullUp(4,A,ON); // STM RST OFF
  expander.setPinMode(0,B,INPUT); //input for abort button

  expander.setPinPullUp(1,B,OFF);// all leds on
  expander.setPinPullUp(2,B,OFF);
  expander.setPinPullUp(3,B,OFF);
  expander.setPinPullUp(4,B,OFF);
  expander.setPinPullUp(5,B,OFF);
  expander.setPinPullUp(6,B,OFF);
  expander.setPinPullUp(7,B,OFF);


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
  // stm.changeState(State::IDLE);
  vTaskDelete(NULL);

}

void loop() {}

/******************************************************

Example sketch for the MCP23017/MCP23S17/MCP23018/
MCP23S18 library.

The sketch shows how to use the MCP23018. The MCP23018 is working
with I2C. All functions are the identical, except the constructors.
The MCP23x18 ICs can deal with higher currents compared to the MCP23x17
ICs, but can only be used as sink.

Be aware that the I2C is set by the voltage applied to the address pin.

Wiring can be found under the same name (png file) 

written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2

*******************************************************/

// #include <Wire.h>
// #include <MCP23018.h>
// #define RESET_PIN 13 
// #define MCP_ADDRESS 0x20 

// /* There are several ways to create your MCP23018 object:
//  * MCP23018 expander = MCP23018(MCP_ADDRESS)            -> uses Wire / no reset pin (if not needed)
//  * MCP23018 expander = MCP23018(MCP_ADDRESS, RESET_PIN)  -> uses Wire / RESET_PIN
//  * MCP23018 expander = MCP23018(&wire2, MCP_ADDRESS)    -> uses the TwoWire object wire2 / no reset pin
//  * MCP23018 expander = MCP23018(&wire2, MCP_ADDRESS, RESET_PIN) -> all together
//  * Successfully tested with two I2C busses on an ESP32
//  */
 
// MCP23018 expander = MCP23018(MCP_ADDRESS, RESET_PIN);

// int wT = 500; // wT = waiting time

// void setup(){ 
//   Serial.begin(115200);
//   Wire.begin(21, 22, 100E3);
//   if(!expander.Init()){
//     Serial.println("Not connected!");
//     while(1){} 
//   } 
//   Serial.print("CONECTED");
//   delay(wT);
//   // expander.setPortMode(0b11111111, B);   // Port B: all pins are OUTPUT

//   // expander.setAllPins(A,OFF);            // Port A: all pins are LOW
//   // expander.setAllPins(B,OFF);   
//   expander.setPinPullUp(7,B,ON);
//   expander.setPinPullUp(5,A,ON);
//   expander.setPinPullUp(6,A,ON);
//   expander.setPinPullUp(7,A,ON);
//   expander.setPinX(6,B,OUTPUT,ON);
//   expander.setPinX(5,B,OUTPUT,ON);
//   expander.setPinX(4,B,OUTPUT,ON);
//   expander.setPinX(0,B,INPUT, PULLUP);
//   expander.setPinMode(3,B,OUTPUT);
//   expander.setPin(3,B,HIGH);
//   pinMode(27, OUTPUT);
//   digitalWrite(27, HIGH);
//   // expander.setPinX(2,B,OUTPUT,ON);
//   expander.setPinX(1,B,OUTPUT,ON);
//   Serial.print("PORT 6:");Serial.println(expander.getPin(6,B));
//           // Port B: all pins are LOW
//   // expander.setPortMode(0b11111111, A);   // Port A: all pins are OUTPUT -> LEDs are on
//   // delay(wT);
//   // expander.setPortMode(0b00000000, A);   // Port A: all pins are INPUT = LEDs are off
//   // expander.setPortMode(0b00000000, B);   // Port B: all pins are INPUT
//   // delay(wT);
//   // byte portModeValue = 0; // = 0b00000000
//   // for(int i=0; i<8; i++){
//   //   portModeValue += (1<<i); // 0b00000001, 0b00000011, 0b00000111, etc.
//   //   expander.setPortMode(portModeValue, A);
//   //   delay(wT);
//   // }
//   // portModeValue = 0;
//   // for(int i=0; i<8; i++){
//   //   portModeValue += (1<<i); // 0b00000001, 0b00000011, 0b00000111, etc.
//   //   expander.setPortMode(portModeValue, B);
//   //   delay(wT);
//   // }
//   // expander.setPortMode(0,A);           // Port A: all pins are INPUT
//   // expander.setPortMode(0,B);           // Port B: all pins are INPUT 
//   // delay(wT);
//   // expander.setPinMode(6, B, OUTPUT);          // Pin 3 / PORT A is OUTPUT/LOW
//   // expander.setPinMode(5, B, OUTPUT);          // Pin 1 / PORT B is OUTPUT/LOW
//   // delay(wT);
//   // expander.setPortMode(0,A);           // Port A: all pins are INPUT
//   // expander.setPortMode(0,B);           // Port B: all pins are INPUT
//   // expander.setPinX(1,A,OUTPUT,LOW);    // A1 HIGH 
//   // delay(wT);
//   // expander.togglePin(6, B);             // A1 LOW
//   // delay(wT);
//   // expander.togglePin(6, B);             // A1 HIGH
//   // delay(wT); 
//   // // the following two lines are similar to setPinX(2,B,OUTPUT,LOW);
//   // expander.setPinMode(2,B,OUTPUT);     // B2 is OUTPUT/LOW
//   // expander.setPin(5,B,HIGH);            // B2 is still OUTPUT/LOW
//   // delay(wT); 
//   // expander.setPortX(0b10001111,0b10000000,B); // B0-B4: OUTPUT/LOW, B7: OUTPUT, HIGH;
// }

// void loop(){ 


//   if(expander.getPin(0,B)==LOW){

//       expander.setPinPullUp(3,B,ON);
//   }
//   else
//     expander.setPinPullUp(3,B,OFF);
  
//   Serial.print("POOOOOOOOOOOOOOORT  0 "); Serial.println(expander.getPin(0,B));
//   Serial.print("POOOOOOOOOOOOOOORT  3 "); Serial.println(expander.getPin(3,B));


//   // expander.setPinPullUp(2,B,ON);
    
//     // expander.togglePin(3,B);
  

//     // expander.setPinX(2,B,INPUT,LOW);
//     // expander.setPinX(3,B,INPUT,LOW);
//     // expander.setPinX(4,B,INPUT,LOW);;
//     // expander.setPinX(5,B,INPUT,LOW);
//     // expander.setPinX(6,B,INPUT,LOW);
//     // expander.setPinX(5,A,OUTPUT,HIGH);
//     // expander.setPinX(6,A,OUTPUT,HIGH);
//     // expander.setPinX(7,A,OUTPUT,HIGH);
//         // Serial.print("POOOOOOOOOOOOOOORT  3 "); Serial.println(expander.getPin(1,B));

//     // Serial.print("POOOOOOOOOOOOOOORT  2 "); Serial.println(expander.getPin(2,B));
//     // delay(1000);
//     // expander.setPinPullUp(1,B,OFF);
//     // Serial.print("POOOOOOOOOOOOOOORT  3 "); Serial.println(expander.getPin(1,B));

//     // delay(1000);

// } 


