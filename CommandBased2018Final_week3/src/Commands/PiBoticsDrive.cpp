/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "PiBoticsDrive.h"
#include "WPILib.h"
#include "ctre/Phoenix.h"

PiBoticsDrive::PiBoticsDrive() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	//gyro = new ADIS16448_IMU;
	//individually set safety off, necessay or motors stutters
	encoderX = 0.0;
	encoderY = 0.0;
	myRobot->DriveCartesian(0.0,0.0,0.0,0.0);
}

// Called just before this Command runs the first time
void PiBoticsDrive::Initialize() {

	//left motor configured for x direction
	lf->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,0);
	lf->ConfigSetParameter(ParamEnum::eFeedbackNotContinuous, 1, 0x00, 0x00, 0x00);
	lf->SetSelectedSensorPosition(0,0,0);
	//right motor configured for y direction
	rf->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder,0,0);
	rf->ConfigSetParameter(ParamEnum::eFeedbackNotContinuous, 1, 0x00, 0x00, 0x00);
	rf->SetSelectedSensorPosition(0,0,0);
	//myRobot->SetExpiration(50.0);

	myRobot->DriveCartesian(0.0,0.0,0.0,0.0);


}


//overloaded execite for use in autonomous
void PiBoticsDrive::Execute(double x, double y, double z, double gyroZ) {
	//encoderX = lf->GetSelectedSensorPosition(0);
	//encoderY = rf->GetSelectedSensorPosition(0);
	myRobot->DriveCartesian(x,y,z, gyroZ);
}
void PiBoticsDrive::Execute(double x, double y, double z, double gyroZ, bool controller){
	//encoderX = lf->GetSelectedSensorPosition(0);
	//encoderY = rf->GetSelectedSensorPosition(0);
	myRobot->DriveCartesian(x,y,z, gyroZ);

}


// Make this return true when this Command no longer needs to run execute()
bool PiBoticsDrive::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void PiBoticsDrive::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void PiBoticsDrive::Interrupted() {

}
