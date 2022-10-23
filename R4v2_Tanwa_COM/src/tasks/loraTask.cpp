#include "../include/tasks/tasks.h"

static void lora_read_message_and_put_on_queue(void) {
  String rxStr = "";
  uint32_t start_time = millis();
  uint32_t timeout = 250;
  uint32_t available = LoRa.available();
  uint8_t rx_buffer[LORA_RX_FRAME_SIZE];
  rxStr.clear();

  while ((available > 0) && (millis() - start_time < timeout)) {
    rxStr += (char)LoRa.read();
    available -= 1;
  }

  if (millis() - start_time > timeout) {
    Serial.println("Lora timeout");
    return;
  }

  size_t rx_size = rxStr.length();
  if (rx_size < LORA_RX_FRAME_SIZE - 1) {
    memcpy(rx_buffer, rxStr.c_str(), rx_size);
    rx_buffer[rx_size] = '\0';
    xQueueSend(stm.loraRxQueue, (void*)&rx_buffer, 0);
  }
}

void loraTask(void *arg){
  char loraTx[LORA_TX_FRAME_SIZE] = {};
  // char loraRx[LORA_RX_FRAME_SIZE] = {};

  //TanWaControl * tc = static_cast<TanWaControl*>(arg);
  
  vTaskDelay(25 / portTICK_PERIOD_MS);
  
  xSemaphoreTake(stm.spiMutex, portMAX_DELAY);

  LoRa.setSPI(stm.spi);
  LoRa.setPins(LORA_CS, LORA_RS, LORA_D0);
  
  while(LoRa.begin(LORA_FREQ_MHZ * 1E6) == 0){ //DEBUG
    Serial.println("LORA begin error!");
    xSemaphoreGive(stm.spiMutex);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xSemaphoreTake(stm.spiMutex, portMAX_DELAY);
  }

  LoRa.setSignalBandwidth(250E3);
  LoRa.disableCrc();
  LoRa.setSpreadingFactor(7);
  LoRa.setTxPower(14);
  LoRa.setTimeout(10);

  xSemaphoreGive(stm.spiMutex);

  vTaskDelay(100 / portTICK_PERIOD_MS);

  while(1){
    xSemaphoreTake(stm.spiMutex, portMAX_DELAY);
    if (LoRa.parsePacket() != 0) {
      if (LoRa.available()) {
        lora_read_message_and_put_on_queue();
      }
    }
    xSemaphoreGive(stm.spiMutex);

    if(xQueueReceive(stm.loraTxQueue, (void*)&loraTx, 0) == pdTRUE){
      xSemaphoreTake(stm.spiMutex, portMAX_DELAY);
        
      LoRa.beginPacket();
      LoRa.write((uint8_t*) loraTx, strlen(loraTx));
      LoRa.endPacket();

      xSemaphoreGive(stm.spiMutex);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}