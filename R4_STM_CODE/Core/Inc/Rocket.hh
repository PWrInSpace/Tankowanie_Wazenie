#include "L298.hh"
#include "Igniter.hh"
#include "string"
#include "memory"


class Rocket{
	std::shared_ptr<Motor> FillMotor;
	std::shared_ptr<Motor> DeprMotor;
	std::shared_ptr<Motor> QDMotor;
	std::shared_ptr<Igniter> igniter;
public:
	Rocket(std::shared_ptr<Motor> _FillMotor, std::shared_ptr<Motor> _DeprMotor, std::shared_ptr<Motor> _QDMotor,  	std::shared_ptr<Igniter> _igniter);
	void comandHandler(std::string comand);
};
