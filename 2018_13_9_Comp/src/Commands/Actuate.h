#ifndef Actuate_H
#define Actuate_H

#include <Commands/Command.h>
#include "WPILib.h"

class Actuate : public frc::Command {
private:
	DoubleSolenoid *solenoid;
	int port1,port2;
	bool isClimbing = false;
public:
	double count = 0.0;
	Actuate(int x,int y);
	void Initialize();
	void Execute(DoubleSolenoid::Value direction);
	bool IsFinished();
	void End();
	void Interrupted();
	//~Actuate();
};

#endif  // Accutuate_H
