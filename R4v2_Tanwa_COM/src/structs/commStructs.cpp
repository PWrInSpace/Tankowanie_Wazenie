#include "../include/structs/commStructs.h"

void createDataFrame(DataFrame df, char *data){
  size_t loraDataSize;

  loraDataSize = snprintf(NULL, 0, "%d;%d;%0.2f;%d;%d;%d;%d;%d;%d;%d;%0.2f;%0.2f;%d;%d;%0.2f;%0.2f;%0.2f",
    df.tanWaState, df.pressureSensor, df.vbat, df.igniterContinouity_1,
    df.igniterContinouity_2, df.motorState_1, df.motorState_2,
    df.motorState_3, df.motorState_4, df.motorState_5,
    df.rocketWeight, df.tankWeight, df.rocketWeightRaw, 
    df.tankWeightRaw, df.thermocouple_1, df.thermocouple_2, df.thermocouple_3) + 1;
  
  char loraFrame[loraDataSize];

  
  snprintf(loraFrame, loraDataSize, "%d;%d;%0.2f;%d;%d;%d;%d;%d;%d;%d;%0.2f;%0.2f;%d;%d;%0.2f;%0.2f;%0.2f",
    df.tanWaState, df.pressureSensor, df.vbat, df.igniterContinouity_1,
    df.igniterContinouity_2, df.motorState_1, df.motorState_2,
    df.motorState_3, df.motorState_4, df.motorState_5,
    df.rocketWeight, df.tankWeight, df.rocketWeightRaw, 
    df.tankWeightRaw, df.thermocouple_1, df.thermocouple_2, df.thermocouple_3);//10
  
  strcpy(data, DATA_PREFIX);
  strcat(data, loraFrame);
  strcat(data, "\n");
}