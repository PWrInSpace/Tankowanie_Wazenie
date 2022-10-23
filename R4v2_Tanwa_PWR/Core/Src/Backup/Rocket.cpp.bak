#include "Rocket.hh"
/*
Rocket::Rocket(std::shared_ptr<Motor> FillMotor_, std::shared_ptr<Motor> DeprMotor_,
				std::shared_ptr<Motor> QDMotor_,
:	FillMotor(FillMotor_), DeprMotor(DeprMotor_), QDMotor(QDMotor_), PQDMotor(PQDMotor_),
	Volt(Volt_), RocketState(RocketStateInit)
{
		SetRocketState(RocketStateInit);
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
__attribute__((optimize("O0")))
void Rocket::RocketCommandHandler(const cString & Input){
	std::string_view comand(Input);
	float tempNumber = 3000;
	if(std::isdigit(comand[5]))
		tempNumber = std::stof(comand.data() + 5);
	//auto result = std::find(comand.begin(), comand.end(), ';');
	//result++;
	//if(std::isdigit(*result)){
	//		tempNumber = std::stof(result);
	//}
	//std::from_chars(comand.data() + 5, comand.data() + comand.size(), tempNumber); //need to find compiler settings for this
	if(comand.substr(0, 4) == "STAT") // state'y
		SetRocketState((uint8_t)(comand[7] - '0'));
	if(comand.substr(0, 4) == "ABRT") // state'y
			SetRocketState(RocketStateAbort);
	else if(comand.substr(0, 4) == "DRST")
		HAL_NVIC_SystemReset();
	else if(comand.substr(0, 2) == "DZ"){	//zawory
		if(comand[2] == 'T');
	//		FillMotor->ValveCommandHandler(comand[3], tempNumber);
		else if(comand[2] == 'D');
	//		DeprMotor->ValveCommandHandler(comand[3], tempNumber);
		else if(comand[2] == 'Q');
	//		QDMotor->ValveCommandHandler(comand[3], tempNumber);
		else if(comand[2] == 'P');
	//		PQDMotor->ValveCommandHandler(comand[3], tempNumber);
	}
}


template void Rocket::RocketCommandHandler(const std::string &);
template void Rocket::RocketCommandHandler(const std::string_view &);
*/
