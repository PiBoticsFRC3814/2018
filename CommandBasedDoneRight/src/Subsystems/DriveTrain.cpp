/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "DriveTrain.h"
#include "../RobotMap.h"
#include "Commands/JoystickMecumDrive.h"
#include "ctre/Phoenix.h"

DriveTrain::DriveTrain() : Subsystem("DriveTrain") {
	WPI_TalonSRX *lf = new WPI_TalonSRX(1);
	WPI_TalonSRX *lr = new WPI_TalonSRX(2);
	WPI_TalonSRX *rf = new WPI_TalonSRX(3);
	WPI_TalonSRX *rr = new WPI_TalonSRX(4);

	drive = std::make_unique<frc::RobotDrive>(lf, lr, rf, rr);
}

void DriveTrain::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	// SetDefaultCommand(new MySpecialCommand());
	SetDefaultCommand(new JoystickMecumDrive());
}

void DriveTrain::MecumDrive(double X, double Y, double Z, double GyroZ)
{
	drive->MecanumDrive_Cartesian(X, Y, Z, GyroZ);
}
