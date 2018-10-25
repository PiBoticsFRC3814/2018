#include "Commands/Auto_3.h"

#include <Commands/Command.h>
#include <SmartDashboard/SmartDashboard.h>
#include "ADIS16448_IMU.h"
#include "ctre/Phoenix.h"
#include "WPILib.h"

Auto_3::Auto_3() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
}

// Called just before this Command runs the first time
void Auto_3::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void Auto_3::Execute() {

	//go to the left or right based of data, then forward
	//right-side auto



	//if (autoCode->SelectData() == "RRBB")
	 //{
		/*time->Start();
			if (time->Get() <=1.0)
			{
			autoElev->Execute(-1.0);
			}
			else
			{
			autoElev->Execute(0.0);
			}
*/
/*		if (!elevUp){
			autoElev->Execute(-1.0);
			Wait(1.0);
			autoElev->Execute(0.0);
			elevUp = true;
			}

		 if (myRobot->encoderY <= dist1)
		 {
			 myRobot->Execute(0.0,0.15,0.0,0.0);
		 }
		 else if (myRobot->encoderX <= dist2)
		 {
			 myRobot->Execute(0.15,0.0,0.0,0.0);
		 }
		 else if (myRobot->encoderY <= dist3+dist1)//needs to be added because enc does not reset after dist1
		 {
			 myRobot->Execute(0.0,0.15,0.0,0.0);
		 }
		 else
			 myRobot->Execute(0.0,0.0,0.0,0.0);

		 autoGrabber->Execute(DoubleSolenoid::kReverse);
	 }

	//left-side auto
	if (autoCode->SelectData() == "BBRR")
		 {
			 if (myRobot->encoderY <= dist1)
			 {
				 myRobot->Execute(0.0,0.15,0.0,0.0);
			 }
			 else if (myRobot->encoderX <=dist2)
			 {
				 myRobot->Execute(-0.15,0.0,0.0,0.0);
			 }
			 else if (myRobot->encoderY <=dist3+dist1)//needs to be added because enc does not reset after dist1
			 {
				 myRobot->Execute(0.0,0.15,0.0,0.0);
			 }
			 else
				 myRobot->Execute(0.0,0.0,0.0,0.0);

			 autoGrabber->Execute(DoubleSolenoid::kReverse);
		 }
*/
}

// Make this return true when this Command no longer needs to run execute()
bool Auto_3::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void Auto_3::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Auto_3::Interrupted() {

}
