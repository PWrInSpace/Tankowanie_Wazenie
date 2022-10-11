#include "../include/tasks/tasks.h"

bool once_upon_a_time = true;
void buzzerTask(void *arg){
    while(1){
        if(( analogRead(IGN_TEST_CON_1) > 1000 ||
        analogRead(IGN_TEST_CON_2) > 1000)&& (StateMachine::getCurrentState() <= ARMED_TO_LAUNCH && StateMachine::getCurrentState() != ABORT))
        {
            // beepBoop(200, 1, SPEAKER);
            // beepBoop(200, 1, BUZZER);
            int E1 = 1318;
            int E2 = 2637;
            int F1 = 1479;
            int F2 = 2960;
            int G1 = 1568;
            int G2 = 3135;

            int delay1 = 200;
            int delay2 = 500;
            // // beepBoop(50, 1, SPEAKER);
            // beepBoop(20, 1, BUZZER);
            // ledcWriteTone(0, 3000);
            // ledcWrite(0, 255);
            if (once_upon_a_time == true){
                once_upon_a_time = false;
                ledcWriteTone(0, E1);
                ledcWrite(0, 255);
                vTaskDelay(delay1 / portTICK_PERIOD_MS);

                ledcWriteTone(0, E2);
                vTaskDelay(delay1 / portTICK_PERIOD_MS);

                ledcWriteTone(0, F1);
                vTaskDelay(delay1 / portTICK_PERIOD_MS);

                ledcWriteTone(0, F2);
                vTaskDelay(delay1 / portTICK_PERIOD_MS);

                ledcWriteTone(0, G1);
                vTaskDelay(delay1 / portTICK_PERIOD_MS);

                ledcWriteTone(0, G2);
                vTaskDelay(delay1 / portTICK_PERIOD_MS);
                ledcWrite(0, 0);
                vTaskDelay(300 / portTICK_PERIOD_MS);
            }

            ledcWriteTone(0, G1);
            ledcWrite(0, 255);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, G2);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, F1);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, F2);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, E1);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, E2);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, E1);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, E2);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, E1);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, E2);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, F1);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, F2);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, G1);
            // ledcWrite(0, 255);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);

            ledcWriteTone(0, G2);
            vTaskDelay(delay1 / portTICK_PERIOD_MS);
            ledcWrite(0, 0);
            vTaskDelay(300 / portTICK_PERIOD_MS);
        }

        else if (( analogRead(IGN_TEST_CON_1) > 1000 ||
           analogRead(IGN_TEST_CON_2) > 1000) && (StateMachine::getCurrentState() >= ARMED_TO_LAUNCH && StateMachine::getCurrentState() != ABORT))
        {
            // beepBoop(100, 1, SPEAKER);
            beepBoop(50, 1, BUZZER);
            ledcWriteTone(0, 3000);
            ledcWrite(0, 255);
            vTaskDelay(50 / portTICK_PERIOD_MS);
            ledcWrite(0, 0);
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }

        else if (StateMachine::getCurrentState() == ABORT)
        {
            // beepBoop(50, 1, SPEAKER);
            beepBoop(20, 1, BUZZER);

            ledcWriteTone(0, 3000);
            ledcWrite(0, 255);
        }
            

        vTaskDelay(10 / portTICK_PERIOD_MS);

    }
}