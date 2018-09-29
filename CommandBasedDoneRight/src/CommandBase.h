/*
 * CommandBase.h
 *
 *  Created on: Jul 21, 2018
 *      Author: PiBotics
 */

#ifndef COMMANDBASE_H_
#define COMMANDBASE_H_

#include "memory"
#include "string"
#include "Commands/Command.h"
#include "OI.h"
#include "Subsystems/DriveTrain.h"
#include "Subsystems/DriveDirection.h"
#include "Subsystems/Elevator.h"

class CommandBase: public frc::Command {
public:
	CommandBase(const std::string& name);
	CommandBase() = default;

	static std::unique_ptr<DriveTrain> driveTrain;
	static std::unique_ptr<DriveDirection> driveDirection;
	static std::unique_ptr<OI> oi;
	static std::unique_ptr<Elevator> Fire;

};


#endif /* COMMANDBASE_H_ */
