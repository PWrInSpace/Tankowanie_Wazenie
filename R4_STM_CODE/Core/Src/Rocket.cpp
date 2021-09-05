#include "Rocket.hh"

Rocket::Rocket(std::shared_ptr<Motor> _FillMotor, std::shared_ptr<Motor> _DeprMotor,
				std::shared_ptr<Motor> _QDMotor, std::shared_ptr<Igniter> _igniter,
				std::shared_ptr<HX711> _RocketWeight, std::shared_ptr<HX711> _TankWeight,  std::shared_ptr<Motor> _PQDMotor){
	FillMotor =_FillMotor;
	DeprMotor =_DeprMotor;
	QDMotor =_QDMotor;
	PQDMotor =_PQDMotor;
	igniter = _igniter;
	RocketWeight = _RocketWeight;
	TankWeight = _TankWeight;
	currState = Init;
}
	
uint16_t Rocket::getCurrState(){
	return currState;
}

void Rocket::comandHandler(std::string comand){
	if(comand.substr(0, 4) == "STAT") // state'y
		currState = (state)(comand[7] - 48);
	else if(comand.substr(0, 4) == "DSTA" && currState == Countdown)  //FIRE
		igniter->FIRE();
	else if(comand.substr(0, 3) == "DWC"){ //calibration
		if(comand.substr(3, 1) == "R")
			RocketWeight->initialCalibration(std::stoi(comand.substr(5, 6)));
		else if(comand.substr(3, 1) == "T"){
			TankWeight->initialCalibration(std::stoi(comand.substr(5, 6)));
		}
	}
	else if(comand.substr(0, 3) == "DWO"){	//wagi
		if(comand.substr(3, 1) == "R")
			RocketWeight->addToOffset(std::stoi(comand.substr(5, 8)));
		else if(comand.substr(3, 1) == "T")
			TankWeight->addToOffset(std::stoi(comand.substr(5, 8)));
	}
	else if(comand.substr(0, 3) == "DWR"){ //wagi
		if(comand.substr(3, 1) == "R")
			RocketWeight->setBitsToGramRatio(std::stoi(comand.substr(5, 8)));
		else if(comand.substr(3, 1) == "T")
			TankWeight->setBitsToGramRatio(std::stoi(comand.substr(5, 8)));
	}
	else if(comand.substr(0, 2) == "DZ"){	//zawory
		if(comand.substr(2, 1) == "T")
			FillMotor->handleComand(comand.substr(3, 1));
		else if(comand.substr(2, 1) == "O")
			DeprMotor->handleComand(comand.substr(3, 1));
		else if(comand.substr(2, 1) == "Q")
			QDMotor->handleComand(comand.substr(3, 1));
		else if(comand.substr(2, 1) == "D")
			PQDMotor->handleComand(comand.substr(3, 1));
	}
}

std::string Rocket::getInfo(){ //not tested
	char bufx[10];
	std::string tmp(std::to_string(currState));
	tmp.insert(tmp.length(), ";");
	tmp.insert(tmp.length(), std::to_string(igniter->isConnected()));
	tmp.insert(tmp.length(), ";");
	tmp.insert(tmp.length(), std::to_string(FillMotor->getStatus()));
	tmp.insert(tmp.length(), ";");
	tmp.insert(tmp.length(), std::to_string(DeprMotor->getStatus()));
	tmp.insert(tmp.length(), ";");
	tmp.insert(tmp.length(), std::to_string(QDMotor->getStatus()));
	tmp.insert(tmp.length(), ";");
	std::sprintf(bufx, "%.1f", RocketWeight->getWeigthInKilogramsWithOffset());
	tmp.insert(tmp.length(), bufx);
	tmp.insert(tmp.length(), ";");
	std::sprintf(bufx, "%.1f", TankWeight->getWeigthInKilogramsWithOffset());
	tmp.insert(tmp.length(), bufx);
	return tmp;
}
