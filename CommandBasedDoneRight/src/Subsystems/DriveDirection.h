/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <Commands/Subsystem.h>
#include <ADIS16448_IMU.h>

class DriveDirection : public frc::Subsystem {
private:


	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities

public:
	DriveDirection();

	ADIS16448_IMU *gyro;

	void InitDefaultCommand() override;
	void ResetGyro();
};

