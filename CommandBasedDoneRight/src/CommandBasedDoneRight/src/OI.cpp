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

	joy = std::make_unique<frc::Joystick>(RobotMap::JOY);
	joyButton = std::make_unique<frc::Joystick>(RobotMap::JOYBUTTON);
	xbox = std::make_unique<frc::XboxController>(RobotMap::XBOX);
	cannonForward = std::make_unique<frc::JoystickButton>(xbox.get(), 5);
	cannonReverse = std::make_unique<frc::JoystickButton>(xbox.get(), 6);

	cannonForward->WhenPressed(new FireTheCannon());
	cannonForward->WhenReleased(new DontFireTheCannon());



	// Process operator interface input here.
}
