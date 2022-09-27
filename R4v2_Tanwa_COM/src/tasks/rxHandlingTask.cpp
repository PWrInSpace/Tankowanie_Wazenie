#include "../include/tasks/tasks.h"
#include <string>
#include <iostream>

using namespace std;
// extern MCP23017 expander;
extern char data[SD_FRAME_SIZE];

enum FrameStates {
    PLSS_,
    TANK_,
    DEPR_,
    ARMM_,
    ARM_,
    DISARM_,
    IGNITER_,
    TARE_RCK_,
    CALIBRATE_RCK_,
    TARE_TANK_,
    CALIBRATE_TANK_,
    SOFT_RESTART_ESP_,
    SOFT_RESTART_STM_,
    SET_CAL_FACTOR_,
    STAT_,
    HOLD_IN_,
    HOLD_OUT_,
    ABRT_,
    INVALID
};


FrameStates resolveOption(string input) {
    if( input == "PLSS" ) return PLSS_;
    if( input == "TANK" ) return TANK_;
    if( input == "DEPR" ) return DEPR_;
    if( input == "ARMM" ) return ARMM_;
    if( input == "ARM" ) return ARM_;
    if( input == "DISARM" ) return DISARM_;
    if( input == "IGNITER" ) return IGNITER_;
    if( input == "TARE_RCK" ) return TARE_RCK_;
    if( input == "CALIBRATE_RCK" ) return CALIBRATE_RCK_;
    if( input == "TARE_TANK" ) return TARE_TANK_;
    if( input == "CALIBRATE_TANK" ) return CALIBRATE_TANK_;
    if( input == "SOFT_RESTART_ESP" ) return SOFT_RESTART_ESP_;
    if( input == "SOFT_RESTART_STM" ) return SOFT_RESTART_STM_;
    if( input == "SET_CAL_FACTOR" ) return SET_CAL_FACTOR_;
    if( input == "STAT" ) return STAT_;
    if( input == "HOLD_IN" ) return HOLD_IN_;
    if( input == "HOLD_OUT" ) return HOLD_OUT_;
    if( input == "ABRT" ) return ABRT_;
    //...
    return INVALID;
 }


void rxHandlingTask(void* arg){
  TxData espNowCommand;
  char loraRx[LORA_RX_FRAME_SIZE];

  while(1){

    //ESPNOW
    if(xQueueReceive(stm.espNowRxQueue, (void*)&espNowCommand, 0) == pdTRUE){
      Serial.print("ESP NOW: ");
      Serial.println(espNowCommand.command);
      Serial.println(espNowCommand.commandValue);

      switch(espNowCommand.command){
        case PLSS_:
          if(xQueueSend(stm.loraTxQueue, (void*)data, 0) == pdTRUE){
            Serial.print("ESP NOW SEND VIA LORA: ");
            // Serial.println(data);
          }
          break;

        case IGNITER:
          digitalWrite(FIRE1, HIGH);
          digitalWrite(FIRE2, HIGH);
          break;

        case TARE_RCK:
          rckWeight.tare();
          break;
        
        case CALIBRATE_RCK:
          rckWeight.CustomCalibration(espNowCommand.commandValue);
          break;

        case TARE_TANK:
          tankWeight.tare();
          break;

        case CALIBRATE_TANK:
          tankWeight.CustomCalibration(espNowCommand.commandValue);
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
          //RESET STM
          // pwrCom.sendCommandMotor(0, RESET_COMMAND);
          break;

        default:
          xSemaphoreTake(stm.i2cMutex, pdTRUE);
          pwrCom.sendCommand(&espNowCommand);
          xSemaphoreGive(stm.i2cMutex);
          break;
      }
    }

    //LORA 
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

            case PLSS_:
              if(xQueueSend(stm.loraTxQueue, (void*)data, 0) == pdTRUE){
                Serial.print("ESP NOW SEND VIA LORA: ");
                // Serial.println(data);//debug
              }
              break;

            case TANK_:
              xSemaphoreTake(stm.i2cMutex, pdTRUE);
              pwrCom.sendCommandMotor(MOTOR_FILL, atoi(frame_array[2].c_str()),atoi(frame_array[3].c_str()));
              xSemaphoreGive(stm.i2cMutex);
              printf("MOTOOOR FILL\n");
              break;
            
            case DEPR_:
              xSemaphoreTake(stm.i2cMutex, pdTRUE);
              pwrCom.sendCommandMotor(MOTOR_DEPR, atoi(frame_array[2].c_str()),atoi(frame_array[3].c_str()));
              xSemaphoreGive(stm.i2cMutex);
              printf("MOTOOOOR DEPR\n");
              break;

            case ARMM_:
              xSemaphoreTake(stm.i2cMutex, pdTRUE);
              pwrCom.sendCommandMotor(MOTOR_QUICK_DISCONNECT, atoi(frame_array[2].c_str()),atoi(frame_array[3].c_str()));
              xSemaphoreGive(stm.i2cMutex);
              printf("MOTOOOR QUICK DISCONNECT\n");
              break;

            case ARM_:
              digitalWrite(ARM_PIN, HIGH);
              break;

            case DISARM_:
              digitalWrite(ARM_PIN, LOW);
              break;

            case IGNITER_:
              digitalWrite(FIRE1, HIGH);
              digitalWrite(FIRE2, HIGH);
              break;

             case TARE_RCK_:
              rckWeight.tare();
              break;
            
            case CALIBRATE_RCK_:
              Serial.print("CAL FACTOR ROCKET: "); Serial.println(rckWeight.CustomCalibration(atoi(frame_array[2].c_str())));
              break;

            case TARE_TANK_:
              tankWeight.tare();
              break;

            case CALIBRATE_TANK_:
              // tankWeight.CustomCalibration(atoi(frame_array[3].c_str()),0);
              // tankWeight.CustomCalibration(atoi(frame_array[2].c_str()));
              Serial.print("CAL FACTOR TANK: "); Serial.println(tankWeight.CustomCalibration(atoi(frame_array[2].c_str())));

              break;

            case SOFT_RESTART_ESP_:
              //RESET ESP COMMAND
              ESP.restart();
              break;

            case SOFT_RESTART_STM_:
              //RESET STM
              // pwrCom.sendCommandMotor(0, RESET_COMMAND);
              xSemaphoreTake(stm.i2cMutex, pdTRUE);
              expander.setPinX(4,A,OUTPUT,OFF);
              xSemaphoreGive(stm.i2cMutex);
        
              Serial.println("RESeeeeeeeeeeeeeeeET");
              vTaskDelay(100 / portTICK_PERIOD_MS);
              xSemaphoreTake(stm.i2cMutex, pdTRUE);
              expander.setPinX(4,A,OUTPUT,ON);
              xSemaphoreGive(stm.i2cMutex);

              break;

          case SET_CAL_FACTOR_:
            if(frame_array[2]=="RCK"){
              rckWeight.setCalFactor(atoi(frame_array[3].c_str()));
              Serial.print("CAL FACTOR RCK = "); Serial.println(atoi(frame_array[3].c_str()));
            }
            else if (frame_array[2]=="TANK"){
              tankWeight.setCalFactor(atoi(frame_array[3].c_str()));
              Serial.print("CAL FACTOR TANK = "); Serial.println(atoi(frame_array[3].c_str()));
            }
            
            break;

            case STAT_:
              StateMachine::changeStateRequest(static_cast<States>(atoi(frame_array[3].c_str())));
              Serial.println("STATE CHANGE REQUEST");
              break;
              
            case HOLD_IN_:
              Serial.println("hold in");
              if(StateMachine::getCurrentState() != States::HOLD){
                if(StateMachine::changeStateRequest(States::HOLD) == false)
                  Serial.println("ERROR HOLD IN");
              }else
                Serial.println("ERROR HOLD IN");
              break;
          
            case HOLD_OUT_:
              Serial.println("hold out");
              if(StateMachine::getCurrentState() == States::HOLD){
                if(StateMachine::changeStateRequest(States::HOLD) == false)
                  Serial.println("ERROR HOLD OUT");
              }else
                Serial.println("ERROR HOLD OUT");
              break;

            case ABRT_:
              Serial.println("ABORT");
              StateMachine::changeStateRequest(States::ABORT);
              break;

            default:
              break;
          }
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}