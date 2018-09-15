#ifndef ArmRotate_H
#define ArmRotate_H

#include <Commands/Command.h>
#include "ctre/phoenix.h"
#include "WPILib.h"


class ArmRotate : public frc::Command {

	WPI_TalonSRX *Talon = new WPI_TalonSRX(20);
	//double direction;
	//double x;
public:

	ArmRotate();
	void Initialize();
	void Execute(double Direction);
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // ArmRotate_H
