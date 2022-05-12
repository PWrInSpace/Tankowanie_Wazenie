#ifndef NOW_H
#define NOW_H

#include "esp_now.h"
#include "WiFi.h"
#include "../include/structs/commStructs.h"
#include "../include/structs/SoftToolsManagment.h"

extern SoftwareToolsManagment stm;

// Adresy:
const uint8_t adressObc[] = {0x9C, 0x9C, 0x1F, 0xCA, 0x68, 0x44};

// Init:
bool nowInit();

// Dodanie peera:
bool nowAddPeer(const uint8_t* address, uint8_t channel);

// Przerwania:
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

#endif