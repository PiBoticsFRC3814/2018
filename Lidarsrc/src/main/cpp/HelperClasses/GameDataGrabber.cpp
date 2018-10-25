/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "HelperClasses/GameDataGrabber.h"
#include "WPILib.h"
GameDataGrabber::GameDataGrabber() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	gameData = DriverStation::GetInstance().GetGameSpecificMessage();//grab data for auto
}

std::string GameDataGrabber::SelectData() {

	gameData = DriverStation::GetInstance().GetGameSpecificMessage();
	return gameData;
}
