#include "Commands/Auto_2.h"
#include "WPILib.h"
Auto_2::Auto_2() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
}

// Called just before this Command runs the first time
void Auto_2::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void Auto_2::Execute() {

	//go to the left or right based of data, then forward
	//***************
/*		time->Start();
				if (time->Get() <=5.0)
				{
				autoElev->Execute(-1.0);
				}
				else
				{
				autoElev->Execute(0.0);
				}
*/
/*	if (!elevUp){
	autoElev->Execute(-1.0);
	Wait(5.0);
	autoElev->Execute(0.0);
	elevUp = true;
	}
			 if (myRobot->encoderY <= dist4)
			 {
				 myRobot->Execute(0.0,0.5,0.0,0.0);
			 }
			 else if (myRobot->encoderX <= dist5)
			 {
				 myRobot->Execute(0.5,0.0,0.0,0.0);
			 }
	//right-side auto
	if (autoCode->SelectData() == "RRBB")
	 {
		if (myRobot->encoderX <= dist5+dist6)//needs to be added because enc does not reset after dist1
		 {
			 myRobot->Execute(0.5,0.0,0.0,0.0);
		 }
		 else if (myRobot->encoderY <= dist4+dist7)
		 {
			 myRobot->Execute(0.0,0.5,0.0,0.0);
		 }
		 else
			 myRobot->Execute(0.0,0.0,0.0,0.0);

		 autoGrabber->Execute(DoubleSolenoid::kReverse);
	 }

	//left-side auto
	if (autoCode->SelectData() == "BBRR")
		 {
			if (myRobot->encoderY <=dist4+dist7)//needs to be added because enc does not reset after dist1
			 {
				 myRobot->Execute(0.0,0.50,0.0,0.0);
			 }
			 else
				 myRobot->Execute(0.0,0.0,0.0,0.0);

			autoGrabber->Execute(DoubleSolenoid::kReverse);
		 }
*/
}

// Make this return true when this Command no longer needs to run execute()
bool Auto_2::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void Auto_2::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Auto_2::Interrupted() {

}
