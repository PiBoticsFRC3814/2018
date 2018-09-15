/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/


#include <Commands/Command.h>
#include <Commands/Scheduler.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <TimedRobot.h>
#include <Timer.h>
#include <string>
#include <iostream>

#include "Commands/PiBoticsDrive.h"
#include "WPILIb.h"
#include "ADIS16448_IMU.h"
#include "ctre/phoenix.h"

#include "Commands/WinchMotor.h"
#include "Commands/Elevator.h"
#include "Commands/ArmRotate.h"
#include "Commands/Actuate.h"

#include "Commands/Auto_1.h"
#include "Commands/Auto_2.h"
#include "Commands/Auto_3.h"
#include "Commands/Auto_4.h"
#include "Commands/DriveController.h"
#include "HelperClasses/AutoHelper.h"
#include "HelperClasses/GameDataGrabber.h"

class Robot : public frc::TimedRobot {

public:

	PiBoticsDrive *piboticsDrive = NULL;
	WinchMotor *winch = NULL;
	Elevator *elev = NULL;
	ArmRotate *tiltCube = NULL;
	CameraServer *cam;

	Actuate* grabber = NULL;
	Actuate* tiltRobot = NULL;
	Actuate* liftEncoder = NULL;
	DoubleSolenoid::Value Order66[3] = {DoubleSolenoid::kForward,DoubleSolenoid::kReverse,DoubleSolenoid::kOff };

	Joystick* driveJoy = new Joystick(0);
	Joystick* buttonJoy = new Joystick(1);
	Joystick* buttonPad = new Joystick(2);
	XboxController* driveCont = new XboxController(3);
	XboxController* buttonCont = new XboxController(4);

	Timer* timer = new Timer();
	Timer* timerDrive = new Timer();

	GameDataGrabber* autoCode = new GameDataGrabber();
	AutoHelper* autoHelper = new AutoHelper();

	ADIS16448_IMU *gyro; //moved to public not sure why it was above public?



	std::string data;
	int FMSselector = 0;
	int DSselector = 0;

	double xin = 0.0, yin = 0.0, zin = 0.0; //initialize variables for deadzone controls
	double deadzoneX = 0.1; //edit these for deadzone controls (x) (side to side)
	double deadzoneY = 0.1; //edit these for deadzone controls	(y) (forward back)
	double deadzoneZ = 0.30; //edit these for deadzone controls (z) (twist)

	double xCont = 0.0, yCont = 0.0, zCont = 0.0;

	double cTime = 0.0; //reset pin on clime timer
	double T1 = 0.0; // timer variables
	double T2 = 0.0;
	bool climbing = false;
	bool datafail = false;
	bool elevatorUp = false;
	bool firstDirection = false;
	bool secondDirection = false;
	bool thirdDirection = false;
	bool timerResetter = false;
	bool timerResetter2 = false;
	double gyroAdjust = 0.0;



	//WPI_TalonSRX *lf = new WPI_TalonSRX(4);
	//WPI_TalonSRX *lr = new WPI_TalonSRX(3);
	//WPI_TalonSRX *rf = new WPI_TalonSRX(1);
	//WPI_TalonSRX *rr = new WPI_TalonSRX(5);
	//MecanumDrive* myRobot = new MecanumDrive(*lf,*lr,*rf,*rr);

	//double time1 = 0.0;
	//double time2 = 0.0;

	//************************************************************************************************************

	void RobotInit() override {

		//drive train
		piboticsDrive = new PiBoticsDrive();
		frc::SmartDashboard::PutNumber("selector", DSselector);


		//motor movement
		winch = new WinchMotor();
		elev = new Elevator();
		tiltCube = new ArmRotate();


		//camera server
		cam->GetInstance()->StartAutomaticCapture();


		//pnuematics movement
		grabber = new Actuate(6,7); //3 and 4 on practice pcm did not work?
		tiltRobot = new Actuate(0,1);
		liftEncoder = new Actuate(4,5);

		gyro = new ADIS16448_IMU;


		data = "";

		m_chooser.AddDefault("Auto Line", &auto1);
		m_chooser.AddObject("Auto Left", &auto2);
		m_chooser.AddObject("Auto Middle", &auto3);
		m_chooser.AddObject("Auto Right", &auto4);
		frc::SmartDashboard::PutData("Auto Selector", &m_chooser);

		c_chooser.AddDefault("Joystick", &joystickChooser);
		c_chooser.AddObject("Controller", &controllerChooser);
		frc::SmartDashboard::PutData("Controller Selector", &c_chooser);
		//customDrive->myRobot->SetSafetyEnabled(false);
		//frc::SmartDashboard::PutData("IMU", gyro);
		gyro->Reset();
		gyro->Calibrate();
		gyro->Reset();

	}

	/**
	 * This function is called once each time the robot enters Disabled
	 * mode.
	 * You can use it to reset any subsystem information you want to clear
	 * when
	 * the robot is disabled.
	 */
	void DisabledInit() override {}

	void DisabledPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
		//frc::SmartDashboard::PutNumber("encCountY", customDrive->rf->GetSelectedSensorPosition(0));
		frc::SmartDashboard::PutData("IMU", gyro);
		//frc::SmartDashboard::PutBoolean("saftey", customDrive->myRobot->IsSafetyEnabled());
		//frc::SmartDashboard::PutNumber("timeout", customDrive->myRobot->GetExpiration());
		frc::SmartDashboard::PutNumber("GyroAngle", gyro->GetAngleZ());

	}

	/**
	 * This autonomous (along with the chooser code above) shows how to
	 * select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString code to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional commands to
	 * the
	 * chooser code above (like the commented example) or additional
	 * comparisons
	 * to the if-else structure below with additional strings & commands.
	 */
	void AutonomousInit() override {
		datafail = false;
		firstDirection = false;
		secondDirection = false;
		thirdDirection = false;
		timerResetter = false;
		elevatorUp = false;
		timer->Reset();
		timer->Start();
		T1 = timer->Get();
		data = autoCode->SelectData();
		gyro->Reset();
		while (T1 < 2.0 && data.length() < 3){
			T1 = timer->Get();
			data = autoCode->SelectData();
			frc::SmartDashboard::PutNumber("Auto Time", T1);
			frc::SmartDashboard::PutNumber("GameData",data.length());
			Wait(0.01);
		}
		frc::SmartDashboard::PutNumber("Auto Time", T1);
		frc::SmartDashboard::PutNumber("GameData",data.length());
		timer->Stop();
		timer->Reset();
		if (data.length() < 3.0)
		{
			datafail = true;
			DriverStation::ReportError("No Data Found");
		}
		if (!datafail)
		{
			if (data == "RLR")
			{
				FMSselector = 1;
				//DriverStation::ReportError("Good Data RLR");
			}
			else if (data == "LRL")
			{
				FMSselector = 2;
				//DriverStation::ReportError("Good Data LRL");
			}
			else if (data == "RRR")
			{
				FMSselector = 3;
				//DriverStation::ReportError("Good Data RRR");
			}
			else if (data == "LLL")
			{
				FMSselector = 4;
				//DriverStation::ReportError("Good Data LLL");
			}
			else
			{
				datafail = true;
				FMSselector = 0;
				//DriverStation::ReportError("data received not expected");
			}

			if(m_chooser.GetSelected() == &auto1)
			{
				DSselector = 1;
				//DriverStation::ReportError("Good Data Line");
			}
			else if (m_chooser.GetSelected() == &auto2)
			{
				DSselector = 2;
				//DriverStation::ReportError("Good Data Left");
			}
			else if (m_chooser.GetSelected() == &auto3)
			{
				DSselector = 3;
				//DriverStation::ReportError("Good Data Middle");
			}
			else if (m_chooser.GetSelected() == &auto4)
			{
				DSselector = 4;
				//DriverStation::ReportError("Good Data Right");
			}
			else
			{
				datafail = true;
				DSselector = 0;
				DriverStation::ReportError("No Chooser Data");
			}
		}
		piboticsDrive->lf->SetSelectedSensorPosition(0,0,0);
		piboticsDrive->rf->SetSelectedSensorPosition(0,0,0);
		timer->Reset();
		timer->Start();
	}
	void GyroAdjust (double desiredAngle){  //not used yet need to figure out gyro directions to get proper rotation direction
		if (gyro->GetAngleZ() > desiredAngle + 1)
		{
			gyroAdjust = (0.05)*(gyro->GetAngleZ());
		}
		else if (gyro->GetAngleZ() < desiredAngle - 1)
		{
			gyroAdjust = (0.05)*(gyro->GetAngleZ());
		}
		else
		{
			gyroAdjust = 0.0;
		}

	}
	void AutonomousPeriodic() override {
		frc::Scheduler::GetInstance()->Run();

		frc::SmartDashboard::PutNumber("selector", DSselector);

		GyroAdjust(0.0);
			if (DSselector == 1)
			{
				DriverStation::ReportError("drive forward");
				if (timer->Get() < 3.0) //was 4 now 3
				{
					piboticsDrive->Execute(0.0, 0.5, gyroAdjust, gyro->GetAngleZ());
				}
				else
				{
					piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ());
				}
			}
			if (DSselector == 3) // middle code
			{
				DriverStation::ReportError("Driving Forward and raising elevator");
				if (timer->Get() <= 1.00) //the timer for elevator
				{
					elev->Execute(-1.0); // elevator at full speed
				}
				else
				{
					elev->Execute(0.0);
				}
				if (timer->Get() <= .75) // the timer for driving forward
				{
					piboticsDrive->Execute(0.0, 0.5, gyroAdjust, gyro->GetAngleZ()); // driving forward
				}
				else
				{
					piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ()); // stopping
					firstDirection = true;
				}
				//****************************THIS CODE WORKS********************************************************    THIS CODE WORKS
				if ((FMSselector == 1 || FMSselector == 3) && firstDirection) // right code
				{

					if (timer->Get() <= 2.9 && timer->Get() >= 1.1) // 1.5 seconds going right
					{
						piboticsDrive->Execute(0.65, 0.0,gyroAdjust, gyro->GetAngleZ()); // going right
						DriverStation::ReportError("Driving Right"); // saying going right
					}
					/*if(timer->Get() > 5.0 && timer->Get() < 7.0)
					{
						piboticsDrive->Execute(0.0, 0.0, gyroAdjust, gyro->GetAngleZ());
					}*/
					if (timer->Get() >= 3.0 && timer->Get() <= 6.25) // going forward for the second time
							{
								DriverStation::ReportError("driving forward again"); //telling the drivers we are going forward
								piboticsDrive->Execute(0.0, 0.25, gyroAdjust, gyro->GetAngleZ()); // going forward
							}
					if(timer->Get() >= 6.25)
							{
								piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ()); // stopping
								grabber->Execute(Order66[1]);

							}

				}//******************************************************************************************     THIS CODE WORKS
				else if ((FMSselector == 2 || FMSselector == 4) && firstDirection) // left code
				{

						if (timer->Get() <= 2.7 && timer->Get() >= 1.1) // 1.5 seconds going left
						{
							DriverStation::ReportError("Driving Left");
							piboticsDrive->Execute(-0.65, 0.0, 0.0, gyro->GetAngleZ()); // going left
						}
						/*else
						{
							piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ()); // stopping
						}*/
						if (timer->Get() >= 2.8 && timer->Get() <= 6.15) // going forward for the second time
						{
							DriverStation::ReportError("driving forward again"); //telling the drivers we are going forward
							piboticsDrive->Execute(0.0, 0.25, gyroAdjust, gyro->GetAngleZ()); // going forward
						}
					if(timer->Get() >= 6.15)
						{
							piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ()); // stopping
							grabber->Execute(Order66[1]);

						}
				}
				else if (DSselector == 2) // left auto
				{
					if (FMSselector == 2 || 4)
					{
						DriverStation::ReportError("Driving Forward and raising elevator");
						if (timer->Get() <= 1.50) //the timer for elevator
						{
						elev->Execute(-1.0); // elevator at full speed
						}
						else
						{
						elev->Execute(0.0);
						}
						if (timer->Get() <= 2.0)
						{
							DriverStation::ReportError("Driving Left");
							piboticsDrive->Execute(0.0, 0.5, gyroAdjust, gyro->GetAngleZ());
						}
						if (timer->Get() >= 2.0 && timer->Get() <= 3.0)
						{
							DriverStation::ReportError("Turning");
							piboticsDrive->Execute(0.0, 0.0, gyroAdjust, 90);

						}
						if (timer->Get() >= 3.0 && timer->Get() <= 3.5)
						{
							DriverStation::ReportError("going forward");
							piboticsDrive->Execute(0.0, 0.5, gyroAdjust, gyro->GetAngleZ());
						}
						if (timer->Get() >= 3.5)
						{
							DriverStation::ReportError("stopping and dropping cube");
							piboticsDrive->Execute(0.0, 0.0, gyroAdjust, gyro->GetAngleZ());
							grabber->Execute(Order66[1]);
						}
					}
					else if(FMSselector == 1 ) // scale on left
					{

					}
					else // you have to go to line if fms = 3 on left side
					{
						DriverStation::ReportError("drive forward");
						if (timer->Get() < 3.0) //was 4 now 3
						{
							piboticsDrive->Execute(0.0, 0.5, gyroAdjust, gyro->GetAngleZ());
						}
						else
						{
							piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ());
						}
					}
				}
				else if (DSselector == 4)//right auto
				{
					 if (FMSselector == 1 || 3)// switch for right
					{
						DriverStation::ReportError("Driving Forward and raising elevator");
						if (timer->Get() <= 1.50) //the timer for elevator
						{
							elev->Execute(-1.0); // elevator at full speed
						}
						else
						{
							elev->Execute(0.0);
						}
						if (timer->Get() <= 2.0)
						{
							DriverStation::ReportError("Driving Left");
							piboticsDrive->Execute(0.0, 0.5, gyroAdjust, gyro->GetAngleZ());
						}
						if (timer->Get() >= 2.0 && timer->Get() <= 3.0)
						{
							DriverStation::ReportError("Turning");
							piboticsDrive->Execute(0.0, 0.0, gyroAdjust, -90);
						}
						if (timer->Get() >= 3.0 && timer->Get() <= 3.5)
						{
							DriverStation::ReportError("going forward");
							piboticsDrive->Execute(0.0, 0.5, gyroAdjust, gyro->GetAngleZ());
						}
						if (timer->Get() >= 3.5)
						{
							DriverStation::ReportError("stopping and dropping cube");
							piboticsDrive->Execute(0.0, 0.0, gyroAdjust, gyro->GetAngleZ());
							grabber->Execute(Order66[1]);
						}
					}
					 else if (FMSselector == 2) // scale for right
					 {

					 }
					 else // you have to do line if fms = 4 on the right side
					 {
						 DriverStation::ReportError("drive forward");
						 if (timer->Get() < 3.0) //was 4 now 3
						 {
						 	piboticsDrive->Execute(0.0, 0.5, gyroAdjust, gyro->GetAngleZ());
						 }
						 else
						{
							piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ());
						}
					 }



				}

				else
				{
					DriverStation::ReportError("No FMS GO GET REF");
				}


			}

			if (DSselector == 2)
			{

			}
			if (DSselector == 4)
			{

			}

	}

	void TeleopInit() override {
		// This makes sure that the autonomous stops running when
		// teleop starts running. If you want the autonomous to
		// continue until interrupted by another command, remove
		// this line or comment it out.
		//customDrive->Initialize();

		piboticsDrive->Execute(0.0,0.0,0.0,0.0);
		gyro->Reset();
		gyro->Calibrate();
		gyro->Reset();
		cTime = 0.0;
		climbing = false;
		timerResetter = false;

		//c_controllerCommand = c_chooser.GetSelected();
		//liftEncoder->Execute(Order66[1]);
		//myRobot->SetExpiration(5.0);
		c_controllerCommand = c_chooser.GetSelected();
		piboticsDrive->Initialize();
		liftEncoder->Execute(Order66[1]);
		//myRobot->DriveCartesian(0.0, 0.0, 0.0, 0.0);
	}

	void deadZone(){

		//below is custom deadzSone code.  we check that the joystick values are greater than the deadzone value.
		//if it is greater than the deadzone then it will pass the value to a variable
		//we then reinitialize the variable to a zero value so the speed isn't sudden

		if (driveJoy->GetX() > deadzoneX || driveJoy->GetX() < -deadzoneX){  //x axis deadzone check
			xin = driveJoy->GetX();

			if (xin < 0.0){
				xin = xin + deadzoneX;
				xin = xin*0.9;
			} //re initialize positive x values

			else if (xin > 0.0) {
				xin = xin - deadzoneX;
				xin = xin*0.9;
			} //re initialize negative x values
		}
		else xin = 0; // if we are lower than the deadzone set the value to zero -- this avoids "locking in" a non-zero value

		if (driveJoy->GetY() > deadzoneY || driveJoy->GetY() < -deadzoneY){ //y axis deadzone check
			yin = (driveJoy->GetY());

			if (yin<0.0){
				yin = yin + deadzoneY;
				yin=yin*0.9;
			}//re initialize positive y values

			else if (yin > 0.0) {
				yin = yin - deadzoneY;
				yin=yin*0.9;
			}//re initialize negative y values
		}
		else yin = 0; // if we are lower than the deadzone set the value to zero -- this avoids "locking in" a non-zero value


		if (driveJoy->GetZ()> (deadzoneZ)|| driveJoy->GetZ()< (-deadzoneZ)){ //z axis deadzone check -- note the z axis needed custom values depending on joystick
			zin = driveJoy->GetZ();
			if (zin<0.0){
				zin = zin + deadzoneZ;
				zin = zin * 0.9; //speed nerf for rotation
			}//re initialize positive z values
			else if (zin > 0.0) {
				zin = zin - deadzoneZ;
				zin = zin * 0.9; //speed nerf for rotation
			}//re initialize negative z values
		}
		else zin = 0; // if we are lower than the deadzone set the value to zero -- this avoids "locking in" a non-zero value
	}
	void ControllerNerf(){

		zCont = driveCont->GetRawAxis(2);
		yCont = driveCont->GetRawAxis(1);
		xCont = driveCont->GetRawAxis(0);
		if (zCont <= 0)  {
			zCont = (zCont*zCont)*-1.0;
		}
		else zCont = (zCont*zCont)*1.0;
	}

	void TeleopPeriodic() override
	{
		//frc::SmartDashboard::PutNumber("T1", time1);
		//frc::SmartDashboard::PutNumber("T2", time2);
		frc::SmartDashboard::PutNumber("GyroAngle", gyro->GetAngleZ());
		frc::SmartDashboard::PutNumber("winch current", winch->winchMotor->GetOutputCurrent());
		frc::SmartDashboard::PutNumber("winch timet", cTime);			//drive train stuff
			c_controllerCommand = c_chooser.GetSelected();

			//NOTE comp bot is "X" practice is "O" for wheels.  "O" is the correct oreintation comp both needs changed.
			if (c_controllerCommand == &joystickChooser || c_chooser.GetSelected() == &joystickChooser)
			{
				deadZone();
				piboticsDrive->Execute(xin, -yin, zin, gyro->GetAngleZ());//was negative gyro
			}
			else
			{
				ControllerNerf();
				piboticsDrive->Execute(xCont,-yCont,zCont,gyro->GetAngleZ(),true); // REMOVED THE NEGATIVE GYRO
			}

			if (driveJoy->GetRawButton(1) || driveCont->GetRawButton(7) || driveCont->GetRawButton(9))
					{
						gyro->Reset();
					}

			//Motor Stuff
			//run winch
			if (buttonPad->GetRawButton(4) && buttonPad->GetRawButton(5))
			{
				winch->Execute();
			}
			else
			{
				winch->End();
			}

			//tilt cube
			if (buttonJoy->GetRawButton(10)||buttonCont->GetRawButton(2))
			{
				tiltCube->Execute(1.0);
			}
			//untilt the cube
			else if (buttonJoy->GetRawButton(9)||buttonCont->GetRawButton(3))
			{
				tiltCube->Execute(-1.0);
			}
			else
			{
				tiltCube->End();
			}

			//elevator down
			if (buttonJoy->GetRawButton(4)||buttonCont->GetRawButton(1))//nerfed so we dont derail XV
			{
				elev->Execute(1.0);
			}
			//elevator up
			else if (buttonJoy->GetRawButton(6)||buttonCont->GetRawButton(4))
			{
				elev->Execute(-1.0);
			}
			else
			{
				elev->End();
			}

			//Pneumatics stuff
			//close grabbers
			if (buttonJoy->GetRawButton(12)||buttonCont->GetRawButton(6))
			{
				grabber->Execute(Order66[0]);
			}
			//open grabbers
			if (buttonJoy->GetRawButton(11)||buttonCont->GetRawButton(5))
			{
				grabber->Execute(Order66[1]);
			}

			//tilt the robot
			if (buttonPad->GetRawButton(3) && buttonPad->GetRawButton(6))
			{
				tiltRobot->Execute(Order66[0]);
			}
			//untilt the robot
			if (buttonPad->GetRawButton(1))
			{
				tiltRobot->Execute(Order66[1]);
			}



			//code to pull tilt pins back to robot after climb
			if(winch->winchMotor->GetOutputCurrent() >= 50)
			{
				//cTime ++;
				if (!timerResetter)
					{
					timer->Stop();
					timer->Reset();
					timer->Start();
					timerResetter=true;
					}
			}
			if (timer->Get() >= 1.0)
			{
				climbing = true;
			}
			if(climbing)
			{
				tiltRobot->Execute(Order66[1]);
				cTime = 0.0;
				timer->Stop();
				timer->Reset();
				timerResetter = false;
				climbing = false;
			}


	}


	void TestPeriodic() override {

		frc::SmartDashboard::PutNumber("GyroAngle", gyro->GetAngleZ());
		GyroAdjust(0.0);
		piboticsDrive->Execute(0.0,0.0,gyroAdjust,gyro->GetAngleZ());

	}


private:
	// Have it null by default so that if testing teleop it
	// doesn't have undefined behavior and potentially crash.
	frc::Command* m_autonomousCommand = nullptr;
	Auto_1 auto1;
	Auto_2 auto2;
	Auto_3 auto3;
	Auto_4 auto4;
	frc::SendableChooser<frc::Command*> m_chooser;

	frc::Command* c_controllerCommand = nullptr;
	DriveController joystickChooser;
	DriveController controllerChooser;
	frc::SendableChooser<frc::Command*> c_chooser;
};

START_ROBOT_CLASS(Robot)
