#include "L298.hh"
#include "Igniter.hh"
#include "string"
#include "memory"

enum state {
	Init = 0,
	Idle = 1,
	Fueling = 2,
	Countdown = 3,
	Flight = 4,
	Abort = 5
};

class Rocket{
	std::shared_ptr<Motor> FillMotor;
	std::shared_ptr<Motor> DeprMotor;
	std::shared_ptr<Motor> QDMotor;
	std::shared_ptr<Igniter> igniter;
	volatile state currState;
public:
	Rocket(std::shared_ptr<Motor> _FillMotor, std::shared_ptr<Motor> _DeprMotor, std::shared_ptr<Motor> _QDMotor,  	std::shared_ptr<Igniter> _igniter);
	uint16_t getCurrState();
	void comandHandler(std::string comand);
};
