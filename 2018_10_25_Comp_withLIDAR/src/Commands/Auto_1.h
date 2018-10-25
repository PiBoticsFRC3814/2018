#ifndef Auto_1_H
#define Auto_1_H

//#include "../CommandBase.h"
#include <Commands/Command.h>
#include <Commands/PiBoticsDrive.h>
#include "WPILib.h"
#include "ADIS16448_IMU.h"


class Auto_1 : public frc::Command {

public:
	Auto_1();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // Auto_1_H
