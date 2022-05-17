#include "../include/tasks/tasks.h"


void stateTask(void *arg){
  StateMachine stateMachine(stm.stateTask);
  while(1){

    if(ulTaskNotifyTake(pdTRUE, false)){
      //state init
      switch(stateMachine.getRequestedState()){
        case INIT:
          if(stateMachine.changeStateRequest(INIT))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();

          vTaskDelay(500 / portTICK_PERIOD_MS);
          break;
        case IDLE:
          //Idle state means nothing is going on

          if(stateMachine.changeStateRequest(IDLE))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();

          vTaskDelay(500 / portTICK_PERIOD_MS);
          break;
      
        case ARMED:
          //CHECK THE CONTINUITY OF IGNITERS
          if(stateMachine.changeStateRequest(ARMED))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          
          vTaskDelay(500 / portTICK_PERIOD_MS);
          break;
           
        case FUELING:
          //CAN GO FROM ARMED
          //ALLOW IF INGITERS HAVE CONTINUITY (HARDWARE ARMED)
          //ALLOW FOR VALE MANIPULATION, NO ACCESS TO IGNITER
          if(stateMachine.changeStateRequest(FUELING))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          vTaskDelay(250 / portTICK_PERIOD_MS);
          break;

        case RDY_TO_LAUNCH:
          //
          //Do not allow valve manipulation, do not give access to the igniter
          if(stateMachine.changeStateRequest(RDY_TO_LAUNCH))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          vTaskDelay(500 / portTICK_PERIOD_MS);
          break;

        case COUNTDOWN:
          //ALLOW TO STOP AND GO BACK TO RDY_TO_LAUNCH
          //CAN GO IN IF ARMED AND SOFTWARMED AND IGNITERS HAVE CONTINUITY
          //FIRE THE IGNITER AFTER COUNTDOWN
           if(stateMachine.changeStateRequest(COUNTDOWN))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();

          vTaskDelay(1000 / portTICK_PERIOD_MS);
          break;

        case HOLD:
          //ALLOW TO GO FROM ANY STATE
          //SOFTWARE DISARM, NO ACCESS TO VALVES
          if(stateMachine.changeStateRequest(HOLD))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          break;

        case ABORT:
          //ALLOW TO GO FROM ANY STATE
          //CAN BE CAUSED EITHER BY SOFTWARE OR SAFETY SWITCH ON THE CASE
          //SOFTWARE DISARM IF ARMED, CLOSE FILL AND OPEN DEPR (DISCONNECT QUICK DISCONN?)
          //IF POSSIBLE GIVE MANUAL CONTROL OVER BOARD STEERING THE VENT AND MAIN VALVE
          if(stateMachine.changeStateRequest(ABORT))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          break;

        default: 
          stateMachine.changeStateRejection();
          break;
      }
    }

    //state loop
    switch(StateMachine::getCurrentState()){
      case FUELING:
        break;
      default:
        break;
    }


    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}