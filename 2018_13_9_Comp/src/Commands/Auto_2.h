#ifndef Auto_2_H
#define Auto_2_H

//#include "../CommandBase.h"
#include <Commands/Command.h>
#include <Commands/PiBoticsDrive.h>
#include "WPILib.h"


class Auto_2 : public frc::Command {
public:
	Auto_2();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // Auto_2_H
