#include "L298.hh"
#include "Igniter.hh"
#include "hx711.hh"
#include "string"
#include "memory"
#include "charconv"

enum state {
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
	std::shared_ptr<Igniter> igniter;
	std::shared_ptr<HX711> RocketWeight;
	std::shared_ptr<HX711> TankWeight;
	volatile state currState;
public:
	Rocket() = default;
	Rocket(std::shared_ptr<Motor> _FillMotor, std::shared_ptr<Motor> _DeprMotor,
			std::shared_ptr<Motor> _QDMotor, std::shared_ptr<Igniter> _igniter,
			std::shared_ptr<HX711> _RocketWeight, std::shared_ptr<HX711> _TankWeight,
			std::shared_ptr<Motor> _PQDMotor = nullptr);
	void setCurrState(uint8_t newState);
	uint8_t getCurrState() const;
	std::string getInfo() const;
	template <typename cString>
	void comandHandler(const cString & comand);
};


