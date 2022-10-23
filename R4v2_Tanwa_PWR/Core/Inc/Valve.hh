#ifndef INC_VALVE_HH_
#define INC_VALVE_HH_

#include <cstdint>

enum ValveState {ValveStateClose = 0, ValveStateOpen = 1, ValveStateIDK = 2 , ValveStateVent = 3, ValveStateAttemptToOpen = 4, ValveStateAttemptToClose = 5};
class ValveInterface{
public:
	volatile ValveState State = ValveStateIDK;
	virtual ValveState GetState() {return State;}
	virtual void SetState(ValveState NewState) {State = NewState;}
	virtual void Stop() = 0;
	virtual void Open() = 0;
	virtual void Close() = 0;
};

#endif /* INC_VALVE_HH_ */
