#include "Rocket.hh"

Rocket::Rocket(std::shared_ptr<Motor> _FillMotor, std::shared_ptr<Motor> _DeprMotor, std::shared_ptr<Motor> _QDMotor, std::shared_ptr<Igniter> _igniter){
	FillMotor =_FillMotor;
	DeprMotor =_DeprMotor;
	QDMotor =_QDMotor;
	igniter = _igniter;
}
	
void Rocket::comandHandler(std::string comand){
	if(comand == "XD")
		;//xd
	else
		;//not xd
}