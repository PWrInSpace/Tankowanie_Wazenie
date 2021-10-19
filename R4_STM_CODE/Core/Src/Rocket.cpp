#include "Rocket.hh"

Rocket::Rocket(std::shared_ptr<Motor> FillMotor_, std::shared_ptr<Motor> DeprMotor_,
				std::shared_ptr<Motor> QDMotor_, std::shared_ptr<Igniter> Ignit_,
				std::shared_ptr<Hx711> RocketWeight_, std::shared_ptr<Hx711> TankWeight_, std::shared_ptr<Motor> PQDMotor_)
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
		DeprMotor->Close();
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

	auto result = std::find(comand.begin(), comand.end(), ';');
	result++;
	if(std::isdigit(*result)){
		tempNumber = std::stof(result);
	}
	//std::from_chars(comand.data() + 5, comand.data() + comand.size(), tempNumber); //need to find compiler settings for this
	if(comand.substr(0, 4) == "STAT") // state'y
		SetRocketState((uint8_t)(comand[7] - '0'));
	if(comand.substr(0, 4) == "ABRT") // state'y
			SetRocketState(RocketStateAbort);
	else if(comand.substr(0, 4) == "DRST")
		HAL_NVIC_SystemReset();
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
			FillMotor->ValveCommandHandler(comand[3], 6000);
		else if(comand[2] == 'O')
			DeprMotor->ValveCommandHandler(comand[3]);
		else if(comand[2] == 'Q')
			QDMotor->ValveCommandHandler(comand[3], 3500);
		else if(comand[2] == 'D')
			PQDMotor->ValveCommandHandler(comand[3]);
	}
}

std::string Rocket::GetInfo() const{
	  char const format[] = "%d;%d;%d;%d;%d;%d;%.2f;%.2f;%li;%li";
	  char message[64];
	  auto written = std::snprintf(message, 64, format,
					  RocketState, Ignit->GetStatus(),
					  FillMotor->GetState(), DeprMotor->GetState(),
					  QDMotor->GetState(), PQDMotor->GetState(),
					  RocketWeight->GetWeigthInKilogramsWithOffset(),TankWeight->GetWeigthInKilogramsWithOffset(),
					  RocketWeight->LastRawAverageRead, TankWeight->LastRawAverageRead);
	  if(written > 64 || written < 0){
	    // jakis problem z parsowaniem
	  }
	  return {message};
}

template void Rocket::RocketCommandHandler(const std::string &);
template void Rocket::RocketCommandHandler(const std::string_view &);
