#include "../include/structs/commStructs.h"

void createDataFrame(DataFrame df, char *data){
  size_t loraDataSize;

  loraDataSize = snprintf(NULL, 0, "%d;%0.2f;%d;%d;%d;%d;%d;%d;%d;%0.2f;%0.2f;%d;%d;%0.2f;%0.2f;%0.2f",
    df.tanWaState, df.vbat, df.igniterContinouity[0],
    df.igniterContinouity[1], df.motorState[0], df.motorState[1],
    df.motorState[2], df.motorState[3], df.motorState[4],
    df.rocketWeight, df.butlaWeight, df.rocketWeightRaw, 
    df.butlaWeightRaw, df.thermocouple[0], df.thermocouple[1], df.thermocouple[2]) + 1;
  
  char loraFrame[loraDataSize];

  
  snprintf(loraFrame, loraDataSize, "%d;%0.2f;%d;%d;%d;%d;%d;%d;%d;%0.2f;%0.2f;%d;%d;%0.2f;%0.2f;%0.2f",
    df.tanWaState, df.vbat, df.igniterContinouity[0],
    df.igniterContinouity[1], df.motorState[0], df.motorState[1],
    df.motorState[2], df.motorState[3], df.motorState[4],
    df.rocketWeight, df.butlaWeight, df.rocketWeightRaw, 
    df.butlaWeightRaw, df.thermocouple[0], df.thermocouple[1], df.thermocouple[2]);//10
  
  strcpy(data, DATA_PREFIX);
  strcat(data, loraFrame);
  strcat(data, "\n");
}