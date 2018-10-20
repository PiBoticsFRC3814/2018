#include "Auto_4.h"
#include "ADIS16448_IMU.h"
#include <memory>

#include <Commands/Command.h>
#include <SmartDashboard/SmartDashboard.h>
#include "WPILib.h"




Auto_4::Auto_4() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
}

// Called just before this Command runs the first time
void Auto_4::Initialize() {



}

// Called repeatedly when this Command is scheduled to run
void Auto_4::Execute() {


	//go to the left or right based of data, then forward
	//frc::SmartDashboard::PutNumber("encCountX",myRobot->encoderX);
	//frc::SmartDashboard::PutNumber("encCountY",myRobot->encoderY);
	//***************
	/*time->Start();
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
				 myRobot->Execute(-0.5,0.0,0.0,0.0);
			 }


	//right-side auto
	if (autoCode->SelectData() == "RRBB")
		 {
			if (myRobot->encoderY <=dist4+dist7)//needs to be added because enc does not reset after dist1
			 {
				 myRobot->Execute(0.0,0.50,0.0,0.0);
			 }
			 else
				 myRobot->Execute(0.0,0.0,0.0,0.0);

			 autoGrabber->Execute(DoubleSolenoid::kReverse);
		 }
	//left-side auto
		if (autoCode->SelectData() == "BBRR")
		 {
			if (myRobot->encoderX <= dist5+dist6)//needs to be added because enc does not reset after dist1
			 {
				 myRobot->Execute(-0.5,0.0,0.0,0.0);
			 }
			 else if (myRobot->encoderY <= dist4+dist7)
			 {
				 myRobot->Execute(0.0,0.5,0.0,0.0);
			 }
			 else
				 myRobot->Execute(0.0,0.0,0.0,0.0);

			 autoGrabber->Execute(DoubleSolenoid::kReverse);
		 }
*/
}

// Make this return true when this Command no longer needs to run execute()
bool Auto_4::IsFinished() {
	return false;


}

// Called once after isFinished returns true
void Auto_4::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Auto_4::Interrupted() {

}
