#include "../include/tasks/tasks.h"

void buzzerTask(void *arg){
    while(1){
        if(( analogRead(IGN_TEST_CON_1) > 1000 ||
        analogRead(IGN_TEST_CON_2) > 1000)&& (StateMachine::getCurrentState() != ARMED && StateMachine::getCurrentState() != ABORT))
        {
            beepBoop(200, 1, SPEAKER);
            beepBoop(200, 1, BUZZER);
        }


        else if (( analogRead(IGN_TEST_CON_1) > 1000 ||
           analogRead(IGN_TEST_CON_2) > 1000) && StateMachine::getCurrentState() == ARMED)
        {
            beepBoop(100, 1, SPEAKER);
            beepBoop(100, 1, BUZZER);

        }

        else if (StateMachine::getCurrentState() == ABORT)
        {
            beepBoop(50, 1, SPEAKER);
            beepBoop(50, 1, BUZZER);

        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }


}