/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "DontFireTheCannon.h"
#include "Subsystems/Elevator.h"

DontFireTheCannon::DontFireTheCannon() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	Requires(CommandBase::Fire.get());
}

// Called just before this Command runs the first time
void DontFireTheCannon::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void DontFireTheCannon::Execute() {
	CommandBase::Fire->ElevatorStop();

}

// Make this return true when this Command no longer needs to run execute()
bool DontFireTheCannon::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DontFireTheCannon::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DontFireTheCannon::Interrupted() {

}
