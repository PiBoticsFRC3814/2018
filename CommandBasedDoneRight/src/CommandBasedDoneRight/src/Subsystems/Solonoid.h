/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <Commands/Subsystem.h>
#include "WPILib.h"

class Solonoid : public frc::Subsystem {
private:
	std::shared_ptr<frc::DoubleSolenoid> intakeDoubleSolenoid;
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities

public:
	Solonoid();
	void InitDefaultCommand() override;
	void SolonoidOpen(enum kreverse) override;
	void SolonoidClose(enum kforward) override;

};

