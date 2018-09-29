/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "OI.h"
#include "RobotMap.h"
#include "Commands/FireTheCannon.h"
#include "Commands/FireTheCannonReverse.h"
#include "Commands/DontFireTheCannon.h"

#include <WPILib.h>

OI::OI() {
	joy = std::make_unique<frc::Joystick>(JOY);
	joyButton = std::make_unique<frc::Joystick>(JOYBUTTON);
	xbox = std::make_unique<frc::XboxController>(XBOX);
	cannonForward = std::make_unique<frc::JoystickButton>(xbox.get(), 5);
	cannonReverse = std::make_unique<frc::JoystickButton>(xbox.get(), 6);
	if (cannonForward->WhenPressed)
		{
		new FireTheCannon();
		}
	else
	{
		new DontFireTheCannon();
	}
	if (cannonReverse->WhenPressed)
		{
		new FireTheCannonReverse();
		}
	else
	{
		new DontFireTheCannon();
	}

	// Process operator interface input here.
}
