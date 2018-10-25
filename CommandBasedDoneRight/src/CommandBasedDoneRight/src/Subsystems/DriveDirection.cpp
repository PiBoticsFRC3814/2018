/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "DriveDirection.h"
#include "../RobotMap.h"
#include <ADIS16448_IMU.h>

DriveDirection::DriveDirection() : Subsystem("ExampleSubsystem") {
	gyro = new ADIS16448_IMU;
}

void DriveDirection::InitDefaultCommand() {

}

void ResetGyro (){

}
// Put methods for controlling this subsystem
// here. Call these from Commands.
