#include "CommandBase.h"

#include <Commands/Scheduler.h>

#include "Subsystems/DriveTrain.h"
#include "Subsystems/DriveDirection.h"
#include "Subsystems/Elevator.h"

std::unique_ptr<DriveTrain> CommandBase::driveTrain =
		std::make_unique<DriveTrain>();

std::unique_ptr<DriveDirection> CommandBase::driveDirection =
		std::make_unique<DriveDirection>();

std::unique_ptr<Elevator> CommandBase::Fire =
		std::make_unique<Elevator>();

std::unique_ptr<OI> CommandBase::oi =
		std::make_unique<OI>();

CommandBase::CommandBase(const std::string &name) :
		frc::Command(name) {

}
