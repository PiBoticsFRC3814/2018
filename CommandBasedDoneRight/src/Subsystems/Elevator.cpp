/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Elevator.h"
#include "../RobotMap.h"
#include "ctre/Phoenix.h"

Elevator::Elevator() : Subsystem("ExampleSubsystem") {
	//WPI_TalonSRX *shooter = new WPI_TalonSRX(30);
	//fire = std::make_unique<phoenix::motorcontrol::can::WPI_TalonSRX>(shooter);
	//fire = std::make_unique<WPI_TalonSRX>(shooter);
	shooter = new WPI_TalonSRX(30);

}



void Elevator::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	// SetDefaultCommand(new MySpecialCommand());
	//shooter = new WPI_TalonSRX(30);

}

void Elevator::ElevatorUp()
{
	//fire->Set(direction);
	shooter->Set(-1.0);
}
void Elevator::ElevatorDown()
{
	//fire->Set(direction);
	shooter->Set(1.0);
}

void Elevator::ElevatorStop(){

	shooter->Set(0.0);

}

// Put methods for controlling this subsystem
// here. Call these from Commands.
