/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Grabber.h"
#include "../RobotMap.h"
#include "ctre/Phoenix.h"

Grabber::Grabber() : Subsystem("ExampleSubsystem") {

}

void Grabber::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	// SetDefaultCommand(new MySpecialCommand());
}
/*void Grabber::Compressor()
{
	comp->SetClosedLoopControl(true);
}*/

// Put methods for controlling this subsystem
// here. Call these from Commands.
