/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "JoystickMecumDrive.h"
#include "../Robot.h"
#include "Subsystems/DriveDirection.h"

#include "RobotMap.h"



JoystickMecumDrive::JoystickMecumDrive() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	Requires(CommandBase::driveTrain.get());
	Requires(CommandBase::driveDirection.get());

}

// Called just before this Command runs the first time
void JoystickMecumDrive::Initialize() {


}

// Called repeatedly when this Command is scheduled to run
void JoystickMecumDrive::Execute() {
	CommandBase::driveTrain->MecumDrive(-CommandBase::oi->joy->GetX(),
									    -CommandBase::oi->joy->GetZ(),
									    -CommandBase::oi->joy->GetY(),
										-CommandBase::driveDirection->gyro->GetAngleZ());
	CommandBase::driveTrain->MecumDrive(CommandBase::oi->xbox->GetRawAxis(0),
									    CommandBase::oi->xbox->GetRawAxis(1),
									    CommandBase::oi->xbox->GetRawAxis(4),
										CommandBase::driveDirection->gyro->GetAngleZ());
	frc::SmartDashboard::PutNumber("gyro",CommandBase::driveDirection->gyro->GetAngleZ());
}

// Make this return true when this Command no longer needs to run execute()
bool JoystickMecumDrive::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void JoystickMecumDrive::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void JoystickMecumDrive::Interrupted() {

}
