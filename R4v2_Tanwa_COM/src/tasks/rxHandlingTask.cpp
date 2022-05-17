#include "../include/tasks/tasks.h"

void rxHandlingTask(void* arg){
  TxData espNowCommand;
  char loraRx[LORA_TX_FRAME_SIZE];

  while(1){
    if(xQueueReceive(stm.espNowRxQueue, (void*)&espNowCommand, 0) == pdTRUE){
      Serial.print("ESP NOW: ");
      Serial.println(espNowCommand.command);
    }

    if(xQueueReceive(stm.loraRxQueue, (void*)&loraRx, 0) == pdTRUE){
      Serial.print("LORA: ");
      Serial.println(loraRx);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}