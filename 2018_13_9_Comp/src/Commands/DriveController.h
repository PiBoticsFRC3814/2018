#ifndef DriveController_H
#define DriveController_H

#include <Commands/Command.h>
#include "WPILib.h"


class DriveController : public frc::Command {
private:

public:

	DriveController();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // DriveController_H
