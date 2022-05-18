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

          break;
        case IDLE:
          //Idle state means nothing is going on

          if(digitalRead(ARM_PIN)==LOW &&
           digitalRead(FIRE1)==LOW &&
           digitalRead(FIRE2)==LOW &&
           digitalRead(BUZZER)==LOW &&
           digitalRead(SPEAKER)== LOW &&
           digitalRead(RUNCAM)==LOW)
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();

          break;
           
        case FUELING:
          //CAN GO FROM ARMED
          //ALLOW IF INGITERS HAVE CONTINUITY (HARDWARE ARMED)
          //ALLOW FOR VALE MANIPULATION, NO ACCESS TO IGNITER
          if(digitalRead(IGN_TEST_CON_1)==HIGH &&
           digitalRead(IGN_TEST_CON_2)==HIGH &&
           digitalRead(ARM_PIN)==LOW &&
           digitalRead(FIRE1)==LOW &&
           digitalRead(FIRE2)==LOW &&
           digitalRead(IGN_TEST_CON_1)==HIGH &&
           digitalRead(IGN_TEST_CON_2)==HIGH &&
           digitalRead(BUZZER)==HIGH &&
           digitalRead(SPEAKER)== HIGH &&
           digitalRead(RUNCAM)==HIGH)
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          break;

        case ARMED:
          //CHECK THE CONTINUITY OF IGNITERS
          if(digitalRead(IGN_TEST_CON_1)==HIGH &&
           digitalRead(IGN_TEST_CON_2)==HIGH &&
           digitalRead(ARM_PIN)==HIGH &&
           digitalRead(FIRE1)==LOW &&
           digitalRead(FIRE2)==LOW &&
           digitalRead(IGN_TEST_CON_1)==HIGH &&
           digitalRead(IGN_TEST_CON_2)==HIGH &&
           digitalRead(BUZZER)==HIGH &&
           digitalRead(SPEAKER)== HIGH &&
           digitalRead(RUNCAM)==HIGH)
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();          
          break;

        case RDY_TO_LAUNCH:
          //
          //Do not allow valve manipulation, do not give access to the igniter
          if(digitalRead(IGN_TEST_CON_1)==HIGH &&
           digitalRead(IGN_TEST_CON_2)==HIGH &&
           digitalRead(ARM_PIN)==HIGH &&
           digitalRead(FIRE1)==LOW &&
           digitalRead(FIRE2)==LOW &&
           digitalRead(IGN_TEST_CON_1)==HIGH &&
           digitalRead(IGN_TEST_CON_2)==HIGH &&
           digitalRead(BUZZER)==HIGH &&
           digitalRead(SPEAKER)== HIGH &&
           digitalRead(RUNCAM)==HIGH)
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          break;

        case COUNTDOWN:
          //ALLOW TO STOP AND GO BACK TO RDY_TO_LAUNCH
          //CAN GO IN IF ARMED AND SOFTWARMED AND IGNITERS HAVE CONTINUITY
          //FIRE THE IGNITER AFTER COUNTDOWN
           if(digitalRead(IGN_TEST_CON_1)==HIGH &&
           digitalRead(IGN_TEST_CON_2)==HIGH &&
           digitalRead(ARM_PIN)==HIGH &&
           digitalRead(FIRE1)==LOW &&
           digitalRead(FIRE2)==LOW &&
           digitalRead(IGN_TEST_CON_1)==HIGH &&
           digitalRead(IGN_TEST_CON_2)==HIGH &&
           digitalRead(BUZZER)==HIGH &&
           digitalRead(SPEAKER)== HIGH &&
           digitalRead(RUNCAM)==HIGH)
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();

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
      case INIT:
        // disarm 
        // close fill close depr

        vTaskDelay(500 / portTICK_PERIOD_MS);
        break;
      case IDLE:
        //Idle state means nothing is going on
        vTaskDelay(500 / portTICK_PERIOD_MS);
        break;
    
      
          
      case FUELING:
        //CAN GO FROM ARMED
        //ALLOW IF INGITERS HAVE CONTINUITY (HARDWARE ARMED)
        //ALLOW FOR VALE MANIPULATION, NO ACCESS TO IGNITER
      
        break;


      case ARMED:
        //CHECK THE CONTINUITY OF IGNITERS
        //if armed 
       // soft arm
        
        
        vTaskDelay(500 / portTICK_PERIOD_MS);
        break;

      case RDY_TO_LAUNCH:
        //
        //Do not allow valve manipulation, do not give access to the igniter
       
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


    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}