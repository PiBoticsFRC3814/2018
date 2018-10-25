/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef PiBoticsDrive_H
#define PiBoticsDrive_H

#include <Commands/Command.h>
#include "ctre/Phoenix.h"
#include "WPILib.h"
#include "ADIS16448_IMU.h"

class PiBoticsDrive : public frc::Command
{
public:
		WPI_TalonSRX *lf = new WPI_TalonSRX(4);
		WPI_TalonSRX *lr = new WPI_TalonSRX(3);
		WPI_TalonSRX *rf = new WPI_TalonSRX(1);
		WPI_TalonSRX *rr = new WPI_TalonSRX(5);
		MecanumDrive *myRobot = new MecanumDrive(*lf,*lr,*rf,*rr);

	double encoderX,encoderY;


	PiBoticsDrive();
	//CustomDrive(double x, double y, double z, double gyroZ);
	void Initialize();
	//void Execute();
	void Execute(double x, double y, double z,double gyroZ);
	void Execute(double x, double y, double z,double gyroZ, bool controller);
	bool IsFinished();
	void End();
	void Interrupted();


};

#endif  // PiBoticsDrive_H
