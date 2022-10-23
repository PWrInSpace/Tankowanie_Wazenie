#include <MAX14870.hh>
#include "string"
#include "memory"
#include "charconv"
#include <algorithm>

enum State {
	RocketStateInit = 0,
	RocketStateIdle = 1,
	RocketStateFueling = 2,
	RocketStateCountdown = 3,
	RocketStateFlight = 4,
	RocketStateAbort = 5,
	RocketStateNumOfStates = 6
};

class Rocket{
	std::shared_ptr<Motor> FillMotor;
	std::shared_ptr<Motor> DeprMotor;
	std::shared_ptr<Motor> QDMotor;
	std::shared_ptr<Motor> PQDMotor;
	volatile State RocketState;
public:
	Rocket() = default;
	Rocket(std::shared_ptr<Motor> FillMotor_, std::shared_ptr<Motor> DeprMotor_,
			std::shared_ptr<Motor> QDMotor_);
	void SetRocketState(uint8_t NewState);
	uint8_t GetRocketState() const;
	//std::string GetInfo() const;
	template <typename cString>
	void RocketCommandHandler(const cString & Command);
};


