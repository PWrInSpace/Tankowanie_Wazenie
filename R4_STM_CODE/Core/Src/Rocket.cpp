#include "Rocket.hh"

Rocket::Rocket(std::shared_ptr<Motor> FillMotor_, std::shared_ptr<Motor> DeprMotor_,
				std::shared_ptr<Motor> QDMotor_, std::shared_ptr<Igniter> Ignit_,
				std::shared_ptr<HX711> RocketWeight_, std::shared_ptr<HX711> TankWeight_,  std::shared_ptr<Motor> PQDMotor_)
:	FillMotor(FillMotor_), DeprMotor(DeprMotor_), QDMotor(QDMotor_), PQDMotor(PQDMotor_),
	Ignit(Ignit_), RocketWeight(RocketWeight_), TankWeight(TankWeight_)
{
		RocketState = RocketStateInit;
}
	
void Rocket::SetRocketState(uint8_t NewState){
	if(NewState < RocketStateNumOfStates)
		RocketState = (State)NewState;
	else
		RocketState = RocketStateIdle;

	if(RocketState == RocketStateIdle){
		FillMotor->Close();
	}
	else if(RocketState == RocketStateAbort){
		FillMotor->Close();
		DeprMotor->Open();
	}
}

uint8_t Rocket::GetRocketState() const{
	return RocketState;
}

template <typename cString>
void Rocket::RocketCommandHandler(const cString & Input){
	std::string_view comand(Input);
	float tempNumber = 0;
	if(std::isdigit(comand[5]))
		tempNumber = std::stof(comand.data() + 5);
	//std::from_chars(comand.data() + 5, comand.data() + comand.size(), tempNumber); //need to find compiler settings for this
	if(comand.substr(0, 4) == "STAT"){ // state'y
		SetRocketState((uint8_t)(comand[7] - '0'));
	}
	else if(comand.substr(0, 4) == "DSTA" && RocketState == RocketStateCountdown)  //FIRE
		Ignit->FIRE();
	else if(comand.substr(0, 2) == "DW"){	//wagi
		if(comand[2] == 'R')
			RocketWeight->WeightCommandHandler(comand[3], tempNumber);
		else if(comand[2] == 'T')
			TankWeight->WeightCommandHandler(comand[3], tempNumber);
	}
	else if(comand.substr(0, 2) == "DZ"){	//zawory
		if(comand[2] == 'T')
			FillMotor->MotorCommandHandler(comand[3]);
		else if(comand[2] == 'O')
			DeprMotor->MotorCommandHandler(comand[3]);
		else if(comand[2] == 'Q')
			QDMotor->MotorCommandHandler(comand[3]);
		else if(comand[2] == 'D')
			PQDMotor->MotorCommandHandler(comand[3]);
	}
}

std::string Rocket::GetInfo() const{
	char bufx[15];
	std::string tmp(std::to_string(RocketState) + ";");
	//tmp.resize(50);
	tmp.append(std::to_string(Ignit->isConnected()) + ";");
	tmp.append(std::to_string(FillMotor->GetState()) + ";");
	tmp.append(std::to_string(DeprMotor->GetState()) + ";");
	tmp.append(std::to_string(QDMotor->GetState()) + ";");
	tmp.append(std::to_string(PQDMotor->GetState()) + ";");
	std::sprintf(bufx, "%.2f", RocketWeight->getWeigthInKilogramsWithOffset());
	tmp.append(bufx);
	tmp.append(";");
	std::sprintf(bufx, "%.2f", TankWeight->getWeigthInKilogramsWithOffset());
	tmp.append(bufx);
	return tmp;
}

template void Rocket::RocketCommandHandler(const std::string &);
template void Rocket::RocketCommandHandler(const std::string_view &);