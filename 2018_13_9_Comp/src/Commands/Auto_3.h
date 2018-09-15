#ifndef Auto_3_H
#define Auto_3_H

//#include "../CommandBase.h"

#include <Commands/Command.h>
#include <Commands/Command.h>

class Auto_3 : public frc::Command {

public:
	Auto_3();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // Auto_3_H
