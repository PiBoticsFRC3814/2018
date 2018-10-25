/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "Joystick.h"
#include "XboxController.h"
#include "Buttons/JoystickButton.h"

class OI {
public:
	OI();

	std::unique_ptr<frc::Joystick> joy;
	std::unique_ptr<frc::Joystick> joyButton;
	std::unique_ptr<frc::JoystickButton> button;
	//std::unique_ptr<frc::Joystick> xbox;
	std::unique_ptr<frc::XboxController> xbox;
	std::unique_ptr<frc::JoystickButton> cannonReverse;
	std::unique_ptr<frc::JoystickButton> cannonForward;
	//i have the fire stuff working but while pressed apears to not work right.

};
