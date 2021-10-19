#ifndef INC_VALVE_HH_
#define INC_VALVE_HH_

#include <cstdint>

enum ValveState {ValveStateClose = 0, ValveStateOpen = 1, ValveStateIDK = 2, ValveStateAttemptToClose = 3 , ValveStateAttemptToOpen = 4};

class ValveInterface{
public:
	volatile ValveState State = ValveStateIDK;
	virtual ValveState GetState() {return State;}
	virtual void Stop() = 0;
	virtual void Open(uint32_t Milisecs) = 0;
	virtual void Close(uint32_t Milisecs) = 0;
	virtual void ValveCommandHandler(char Command, uint32_t Milisecs) = 0;
};

#endif /* INC_VALVE_HH_ */
