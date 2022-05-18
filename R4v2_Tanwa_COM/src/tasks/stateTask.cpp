#include "../include/tasks/tasks.h"



void stateTask(void *arg){
  StateMachine stateMachine(stm.stateTask);
  TxData ramka{0,0};
  while(1){

    if(ulTaskNotifyTake(pdTRUE, false)){
      switch(stateMachine.getRequestedState()){
        case IDLE:
          stateMachine.changeStateConfirmation();
          break;
           
        case FUELING:
          if((digitalRead(IGN_TEST_CON_1)==HIGH ||
           digitalRead(IGN_TEST_CON_2)==HIGH) &&
           digitalRead(ARM_PIN)==LOW &&
           digitalRead(FIRE1)==LOW &&
           digitalRead(FIRE2)==LOW )
           
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          break;

        case ARMED:
          if((digitalRead(IGN_TEST_CON_1)==HIGH ||
           digitalRead(IGN_TEST_CON_2)==HIGH) &&
           digitalRead(ARM_PIN)==LOW &&
           digitalRead(FIRE1)==LOW &&
           digitalRead(FIRE2)==LOW )

            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();          
          break;

        case RDY_TO_LAUNCH:
          if((digitalRead(IGN_TEST_CON_1)==HIGH ||
           digitalRead(IGN_TEST_CON_2)==HIGH) &&
           digitalRead(ARM_PIN)==HIGH &&
           digitalRead(FIRE1)==LOW &&
           digitalRead(FIRE2)==LOW)
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          break;

        case COUNTDOWN:
           if((digitalRead(IGN_TEST_CON_1)==HIGH ||
           digitalRead(IGN_TEST_CON_2)==HIGH) &&
           digitalRead(ARM_PIN)==HIGH &&
           digitalRead(FIRE1)==LOW &&
           digitalRead(FIRE2)==LOW)
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();

          break;

        case HOLD:
          stateMachine.changeStateConfirmation();
          break;

        case ABORT:
          stateMachine.changeStateConfirmation();
          break;

        default: 
          stateMachine.changeStateRejection();
          break;
      }
    }

    //state loop
    switch(StateMachine::getCurrentState()){
      case IDLE:
        //Idle state means nothing is going on

        vTaskDelay(500 / portTICK_PERIOD_MS);
        break;
    
      
          
      case FUELING:
        //CAN GO FROM ARMED
        //ALLOW IF INGITERS HAVE CONTINUITY (HARDWARE ARMED)
        //ALLOW FOR VALE MANIPULATION, NO ACCESS TO IGNITER

        digitalWrite(ARM_PIN, LOW);
        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);
        //send msg to close valves?
        ramka.command = 1;
        ramka.commandValue = 0;

        xSemaphoreTake(stm.i2cMutex, pdTRUE);
        pwrCom.sendCommand(&ramka);
        xSemaphoreGive(stm.i2cMutex);

      
        break;


      case ARMED:
        //CHECK THE CONTINUITY OF IGNITERS
        //if armed 
       // soft arm
        digitalWrite(ARM_PIN, HIGH);
        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);
        
        // vTaskDelay(10 / portTICK_PERIOD_MS);
        break;

      case RDY_TO_LAUNCH:
        //
        //Do not allow valve manipulation, do not give access to the igniter
        digitalWrite(ARM_PIN, HIGH);
        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);

        // vTaskDelay(10 / portTICK_PERIOD_MS);
        break;

      case COUNTDOWN:
        //ALLOW TO STOP AND GO BACK TO RDY_TO_LAUNCH
        //CAN GO IN IF ARMED AND SOFTWARMED AND IGNITERS HAVE CONTINUITY
        //FIRE THE IGNITER AFTER COUNTDOWN
        digitalWrite(ARM_PIN, HIGH);
        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);

        // vTaskDelay(10 / portTICK_PERIOD_MS);
        break;

      case HOLD:
        //ALLOW TO GO FROM ANY STATE
        //SOFTWARE DISARM, NO ACCESS TO VALVES
        digitalWrite(ARM_PIN, LOW);
        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);
        //CLOSE VALVES MSG

        break;

      case ABORT:
        //ALLOW TO GO FROM ANY STATE
        //CAN BE CAUSED EITHER BY SOFTWARE OR SAFETY SWITCH ON THE CASE
        //SOFTWARE DISARM IF ARMED, CLOSE FILL AND OPEN DEPR (DISCONNECT QUICK DISCONN?)
        //IF POSSIBLE GIVE MANUAL CONTROL OVER BOARD STEERING THE VENT AND MAIN VALVE
        digitalWrite(ARM_PIN, LOW);
        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);
        //CLOSE FILL, OPEN DEPR, OPEN VENT

        break;

      default: 
        break;
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}