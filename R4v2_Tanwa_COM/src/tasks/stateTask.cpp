#include "../include/tasks/tasks.h"

// extern MCP23017 expander;
extern  PWRData pwrData;
void stateTask(void *arg){
  StateMachine stateMachine(stm.stateTask);
  bool initialCondition = false;
  bool abort_flag = false;
  bool hold_flag = false;
  while(1){

    if(ulTaskNotifyTake(pdTRUE, false)){

      hold_flag = false; // reset hold after changing state (getting out of)
      switch(stateMachine.getRequestedState()){
        case IDLE:
          stateMachine.changeStateConfirmation();
          break;
        case RECOVERY_ARM:
            stateMachine.changeStateConfirmation();
          break;
           
        case FUELING:
          if(( analogRead(IGN_TEST_CON_1) > 1000 ||
            analogRead(IGN_TEST_CON_2) > 1000))

            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          break;

        case ARMED_TO_LAUNCH:
          if(( analogRead(IGN_TEST_CON_1) > 1000 ||
            analogRead(IGN_TEST_CON_2) > 1000))

            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();          
          break;

        case RDY_TO_LAUNCH:
          if(( analogRead(IGN_TEST_CON_1) > 1000 ||
            analogRead(IGN_TEST_CON_2) > 1000))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();
          break;

        case COUNTDOWN:
           if(( analogRead(IGN_TEST_CON_1) > 1000 ||
            analogRead(IGN_TEST_CON_2) > 1000))
            stateMachine.changeStateConfirmation();
          else
            stateMachine.changeStateRejection();

          break;

        case HOLD:
          // piekny przyklad uzycia klasy uniwersalnej -- nie dla debili jak ja ~ molon
          //TxData motorMsg{0,0}; predefined at the beginning of a file
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
        // xSemaphoreTake(stm.i2cMutex, pdTRUE);
        // expander.setPinPullUp(3,B,ON); // indication LED ON
        // xSemaphoreGive(stm.i2cMutex);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        break;

      case RECOVERY_ARM:
        xSemaphoreTake(stm.i2cMutex, pdTRUE);
        expander.setPinPullUp(3,B,ON); // indication LED ON
        xSemaphoreGive(stm.i2cMutex);
        break;
    
      
          
      case FUELING:
        //CAN GO FROM ARMED
        //ALLOW IF INGITERS HAVE CONTINUITY (HARDWARE ARMED)
        //ALLOW FOR VALE MANIPULATION, NO ACCESS TO IGNITER

        digitalWrite(ARM_PIN, LOW);
        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);

        xSemaphoreTake(stm.i2cMutex, pdTRUE);
        expander.setPinPullUp(4,B,ON); // indication LED ON
        xSemaphoreGive(stm.i2cMutex);
        //send msg to close valves?
        // motorMsg.command = MOTOR_FILL;
        // motorMsg.commandValue = CLOSE_VALVE;
        if(initialCondition == false){
          xSemaphoreTake(stm.i2cMutex, pdTRUE);
          pwrCom.sendCommandMotor(MOTOR_FILL, CLOSE_VALVE);
          pwrCom.sendCommandMotor(MOTOR_DEPR, CLOSE_VALVE);
          xSemaphoreGive(stm.i2cMutex);
          initialCondition = true;
        }

        break;


      case ARMED_TO_LAUNCH:
        //CHECK THE CONTINUITY OF IGNITERS
        //if armed 
       // soft arm
        digitalWrite(ARM_PIN, HIGH);
        xSemaphoreTake(stm.i2cMutex, pdTRUE);
        expander.setPinPullUp(5,B,ON); //indication LED ON
        xSemaphoreGive(stm.i2cMutex);

        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);

        
        break;

      case RDY_TO_LAUNCH:
        //
        //Do not allow valve manipulation, do not give access to the igniter
        digitalWrite(ARM_PIN, HIGH);
        
        xSemaphoreTake(stm.i2cMutex, pdTRUE);
        expander.setPinPullUp(6,B,ON); // indication LED ON
        xSemaphoreGive(stm.i2cMutex);

        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);

        break;

      case COUNTDOWN:
        //ALLOW TO STOP AND GO BACK TO RDY_TO_LAUNCH
        //CAN GO IN IF ARMED AND SOFTWARMED AND IGNITERS HAVE CONTINUITY
        //FIRE THE IGNITER AFTER COUNTDOWN
        xSemaphoreTake(stm.i2cMutex, pdTRUE);
        expander.setPinPullUp(6,B,ON);  // indication LED ON
        expander.setPinPullUp(5,B,OFF); 
        expander.setPinPullUp(4,B,OFF); 
        expander.setPinPullUp(3,B,OFF); 
        xSemaphoreGive(stm.i2cMutex);


        digitalWrite(ARM_PIN, HIGH);
        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);

        break;

      case HOLD:
        //ALLOW TO GO FROM ANY STATE
        //SOFTWARE DISARM, NO ACCESS TO VALVES

        digitalWrite(ARM_PIN, LOW);
        digitalWrite(FIRE1, LOW);
        digitalWrite(FIRE2, LOW);

        if(hold_flag == false){

          hold_flag = true;

          for(int i=0; i<5; i++){
           xSemaphoreTake(stm.i2cMutex, pdTRUE);
          pwrCom.sendCommandMotor(MOTOR_FILL, CLOSE_VALVE);
          pwrCom.sendCommandMotor(MOTOR_DEPR, CLOSE_VALVE);
          xSemaphoreGive(stm.i2cMutex);
          vTaskDelay(1000);

          }

        }
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
        if(abort_flag == false){
          abort_flag = true;
           //CLOSE FILL, OPEN DEPR
          xSemaphoreTake(stm.i2cMutex, pdTRUE);
          for(int i = 0; i<15;i++){
              pwrCom.sendCommandMotor(MOTOR_FILL, CLOSE_VALVE);
              vTaskDelay(1000 / portTICK_PERIOD_MS);
          }
        
          if(pwrData.motorState[4]==CLOSE_VALVE)
            for(int i = 0; i<5;i++){
            pwrCom.sendCommandMotor(MOTOR_DEPR, OPEN_VALVE);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
          }
          xSemaphoreGive(stm.i2cMutex);          
        }

        break;

      default: 
        break;
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}