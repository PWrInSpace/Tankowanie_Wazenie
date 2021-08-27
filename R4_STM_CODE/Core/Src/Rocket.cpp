#include "Rocket.hh"

Rocket::Rocket(std::shared_ptr<Motor> _FillMotor, std::shared_ptr<Motor> _DeprMotor, std::shared_ptr<Motor> _QDMotor, std::shared_ptr<Igniter> _igniter){
	FillMotor =_FillMotor;
	DeprMotor =_DeprMotor;
	QDMotor =_QDMotor;
	igniter = _igniter;
	currState = Init;
}
	
uint16_t Rocket::getCurrState(){
	return currState;
}

//
void Rocket::comandHandler(std::string comand){
	if(comand.substr(0, 4) == "STAT")
		currState = (state)(((int) (comand[7])) - 48);
	else if(comand.substr(0, 4) == "DSTA" && currState == Countdown)
		igniter->FIRE();
	else if(comand.substr(0, 3) == "DWC"){
		if(comand.substr(3, 1) == "R")
			;
		else if(comand.substr(3, 1) == "0")
			;
	}
}