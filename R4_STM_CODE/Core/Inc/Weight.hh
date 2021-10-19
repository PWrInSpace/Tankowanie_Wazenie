#ifndef INC_WEIGHT_HH_
#define INC_WEIGHT_HH_

#include <cstdint>

class Weight{
public:
	virtual float GetWeigthInKilogramsWithOffset(uint16_t Times) = 0;
	virtual void Tare() = 0;
	virtual void InitialCalibration(float TestLoadInGrams, uint16_t ValibrationTimeInMilis) = 0;
	virtual void WeightCommandHandler(char Command, float Number) = 0;
};

#endif /* INC_WEIGHT_HH_ */
