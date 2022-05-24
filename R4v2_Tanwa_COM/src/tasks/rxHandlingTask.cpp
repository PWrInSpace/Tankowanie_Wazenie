#include "../include/tasks/tasks.h"
#include <string>
#include <iostream>

using namespace std;

enum FrameStates {
    PLSS,
    TANK,
    DEPR,
    ARMM,
    STAT,
    INVALID
};



FrameStates resolveOption(string input) {
    if( input == "PLSS" ) return PLSS;
    if( input == "TANK" ) return TANK;
    if( input == "DEPR" ) return DEPR;
    if( input == "ARMM" ) return ARMM;
    if( input == "STAT" ) return STAT;
    //...
    return INVALID;
 }


void rxHandlingTask(void* arg){
  TxData espNowCommand;
  char loraRx[LORA_RX_FRAME_SIZE];

  while(1){
    if(xQueueReceive(stm.espNowRxQueue, (void*)&espNowCommand, 0) == pdTRUE){
      Serial.print("ESP NOW: ");
      Serial.println(espNowCommand.command);
        //TODO parser

      xSemaphoreTake(stm.i2cMutex, pdTRUE);
      pwrCom.sendCommand(&espNowCommand);
      xSemaphoreGive(stm.i2cMutex);
    }

    if(xQueueReceive(stm.loraRxQueue, (void*)&loraRx, 0) == pdTRUE){
      Serial.print("LORA: ");
      Serial.println(loraRx);
    
      //TODO parser
      string frame_array [50];
      string loraRx_frame = loraRx;
      string delimiter = ";";
      string frame_elem;
      string frame_function;
      
      frame_elem = loraRx_frame.substr(0, loraRx_frame.find(delimiter));
      frame_array[0] = frame_elem;
      loraRx_frame = loraRx_frame.substr(frame_elem.length());
      loraRx_frame = loraRx_frame.erase(0, 1);
     

      if(frame_array[0]=="R4T" || frame_array[0]=="R4A"){  
         int i = 1;
        do {//decomposition of the frame
          
              frame_elem = loraRx_frame.substr(0, loraRx_frame.find(delimiter));
              frame_array[i] = frame_elem;
              loraRx_frame = loraRx_frame.substr(frame_elem.length());
              loraRx_frame = loraRx_frame.erase(0, 1);
              i++;
              cout<<"lora = "<<frame_elem<<endl;

          } while (loraRx_frame.compare("") != 0);

        
          

          frame_function = frame_array[1];

          switch(resolveOption(frame_function)){
            case PLSS://do wywalenia?
            case TANK:
              xSemaphoreTake(stm.i2cMutex, pdTRUE);
              pwrCom.sendCommandMotor(MOTOR_FILL, atoi(frame_array[2].c_str()),atoi(frame_array[3].c_str()));
              xSemaphoreGive(stm.i2cMutex);
              printf("MOTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOR\n");
              break;
            
            case DEPR:
            case ARMM:
            case STAT:
            default:
              break;
          }
      }

    
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}