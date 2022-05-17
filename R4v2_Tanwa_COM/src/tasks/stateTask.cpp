#include "../include/tasks/tasks.h"


void stateTask(void *arg){
  StateMachine stateMachine(stm.stateTask);
  while(1){

    if(ulTaskNotifyTake(pdTRUE, false)){
      //state init
      switch(stateMachine.getRequestedState()){
        case IDLE:
          //Idle state means nothing is going on

          stateMachine.changeStateConfirmation();
          break;
        case ARMED:
          //CHECK THE CONTINUITY OF IGNITERS
          stateMachine.changeStateConfirmation();
          break;
        case FUELING:
          //CAN GO FROM ARMED
          //ALLOW IF INGITERS HAVE CONTINUITY (HARDWARE ARMED)
          //ALLOW FOR VALE MANIPULATION, NO ACCESS TO IGNITER
          stateMachine.changeStateConfirmation();
          break;
        case RDY_TO_LAUNCH:
          //
          //Do not allow valve manipulation, do not give access to the igniter
          stateMachine.changeStateConfirmation();
          break;
        case COUNTDOWN:
          //ALLOW TO STOP AND GO BACK TO RDY_TO_LAUNCH
          //CAN GO IN IF ARMED AND SOFTWARMED AND IGNITERS HAVE CONTINUITY
          //FIRE THE IGNITER AFTER COUNTDOWN
          stateMachine.changeStateConfirmation();
          break;
        case HOLD:
          //ALLOW TO GO FROM ANY STATE
          //SOFTWARE DISARM, NO ACCESS TO VALVES
          stateMachine.changeStateConfirmation();
          break;
        case ABORT:
          //ALLOW TO GO FROM ANY STATE
          //CAN BE CAUSED EITHER BY SOFTWARE OR SAFETY SWITCH ON THE CASE
          //SOFTWARE DISARM IF ARMED, CLOSE FILL AND OPEN DEPR (DISCONNECT QUICK DISCONN?)
          //IF POSSIBLE GIVE MANUAL CONTROL OVER BOARD STEERING THE VENT AND MAIN VALVE
          stateMachine.changeStateConfirmation();
          break;
        default:
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