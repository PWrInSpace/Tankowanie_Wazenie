#include "../include/tasks/tasks.h"
#include <string>
#include <iostream>

using namespace std;

enum FrameStates {
    PLSS,
    TANK,
    DEPR,
    ARMM,
    ARM,
    DISARM,
    STAT,
    HOLD,
    ABRT,
    INVALID
};



FrameStates resolveOption(string input) {
    if( input == "PLSS" ) return PLSS;
    if( input == "TANK" ) return TANK;
    if( input == "DEPR" ) return DEPR;
    if( input == "ARMM" ) return ARMM;
    if( input == "ARM" ) return ARM;
    if( input == "DISARM" ) return DISARM;
    if( input == "STAT" ) return STAT;
    if( input == "HOLD" ) return HOLD;
    if( input == "ABRT" ) return ABRT;
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
      Serial.println(espNowCommand.commandValue);
        //TODO parser zaplnik, kalibracja, state

      switch(espNowCommand.command){
        case IGNITER:
          if(espNowCommand.commandValue==1)
            digitalWrite(FIRE1, HIGH);
          if(espNowCommand.commandValue==2)
            digitalWrite(FIRE2, HIGH);
          break;

        case TARE_RCK:
          rckWeight.tare();
          break;
        
        case CALIBRATE_RCK:
          rckWeight.CustomCalibration(espNowCommand.commandValue,0);
          break;

        case TARE_TANK:
          tankWeight.tare();
          break;

        case CALIBRATE_TANK:
          tankWeight.CustomCalibration(espNowCommand.commandValue,0);
          break;

        case SOFT_ARM:
          digitalWrite(ARM_PIN, HIGH);
          break;

        case SOFT_DISARM:
          digitalWrite(ARM_PIN, LOW);
          break;
        
        case SOFT_RESTART:
        //RESET ESP COMMAND
          ESP.restart();
          break;

        default:
          xSemaphoreTake(stm.i2cMutex, pdTRUE);
          pwrCom.sendCommand(&espNowCommand);
          xSemaphoreGive(stm.i2cMutex);
          break;
        
      }
      
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
              printf("MOTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOR FILL\n");
              break;
            
            case DEPR:
              xSemaphoreTake(stm.i2cMutex, pdTRUE);
              pwrCom.sendCommandMotor(MOTOR_DEPR, atoi(frame_array[2].c_str()),atoi(frame_array[3].c_str()));
              xSemaphoreGive(stm.i2cMutex);
              printf("MOTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOR DEPR\n");
              break;

            case ARMM:
              xSemaphoreTake(stm.i2cMutex, pdTRUE);
              pwrCom.sendCommandMotor(MOTOR_QUICK_DISCONNECT, atoi(frame_array[2].c_str()),atoi(frame_array[3].c_str()));
              xSemaphoreGive(stm.i2cMutex);
              printf("MOTOOOR QUICK DISCONNECT\n");
              break;

            case ARM:
              digitalWrite(ARM_PIN, HIGH);
              break;

            case DISARM:
              digitalWrite(ARM_PIN, LOW);
              break;

            case STAT:
              stateMachine.changeStateRequest(atoi(frame_array[3].c_str()));
              printf("STATE CHANGE REQUEST");
              break;
              
            case HOLD:
              stateMachine.changeStateRequest(HOLD);
              break;

            case ABRT:
              stateMachine.changeStateRequest(ABORT);
              break;

            default:
              break;
          }
      }

    
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}