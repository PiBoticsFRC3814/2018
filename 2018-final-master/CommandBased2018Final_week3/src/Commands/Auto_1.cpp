#include "Auto_1.h"
#include <memory>
#include <Commands/Command.h>
#include <wpilib.h>




Auto_1::Auto_1() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
}

// Called just before this Command runs the first time
void Auto_1::Initialize() {


}

// Called repeatedly when this Command is scheduled to run
void Auto_1::Execute() {

}


// Make this return true when this Command no longer needs to run execute()
bool Auto_1::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void Auto_1::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Auto_1::Interrupted() {

}
