#include "../include/tasks/tasks.h"


void stateTask(void *arg){
  StateMachine stateMachine(stm.stateTask);
  while(1){
    
    if(ulTaskNotifyTake(pdTRUE, false)){
      //state init
      switch(stateMachine.getRequestedState()){
        case IDLE:

          stateMachine.changeStateConfirmation();
          break;
        case ARMED:

          stateMachine.changeStateConfirmation();
          break;
        case FUELING:

          stateMachine.changeStateConfirmation();
          break;
        case RDY_TO_LAUNCH:

          stateMachine.changeStateConfirmation();
          break;
        case COUNTDOWN:

          stateMachine.changeStateConfirmation();
          break;
        case HOLD:

          stateMachine.changeStateConfirmation();
          break;
        case ABORT:

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



