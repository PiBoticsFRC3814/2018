/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/


#include "Commands/Command.h"
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

	Counter *LIDAR = new Counter(6);

	Actuate* grabber = NULL;
	Actuate* tiltRobot = NULL;
	//Actuate* liftEncoder = NULL;
	Actuate* tiltArm = NULL;
	DoubleSolenoid::Value solenoidFire[3] = {DoubleSolenoid::kForward,DoubleSolenoid::kReverse,DoubleSolenoid::kOff };

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
	bool fourthDirection = false;
	bool timerResetter = false;
	bool timerResetter2 = false;
	bool firstRotation = false;
	bool secondRotation = false;
	bool thirdRotation = false;
	bool fourthDistance = false;
	double gyroAdjust = 0.0;

	double distance1 = 50.0; //distance from starting wall to scale center
	double distance2 = 12.0; //distance from side wall to scale
	double distance3 = 12.0; //distance from starting wall to between scale and switch
	double distance4 = 12.0; // distance from side wall to change field (move to other side of scale)
	double distError = 0.0;
	int LIDARCalibrationOffset = -6; // In CENTIMETERS
	int warningCount = 5;
	double distance_cm = 0.0;
	double distance_in = 0.0;
	double robotDistance = 0.0;
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



		//camera server
		cam->GetInstance()->StartAutomaticCapture();


		//pnuematics movement
		grabber = new Actuate(6,7); //3 and 4 on practice pcm did not work?
		tiltRobot = new Actuate(0,1);
		//liftEncoder = new Actuate(4,5);
		//tiltCube = new ArmRotate(2,3);
		tiltArm = new Actuate(4,5);

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

		LIDAR->Reset();
		LIDAR->SetMaxPeriod(1.0);
		LIDAR->SetSemiPeriodMode(true);
		LIDAR->Reset();

		gyro->Reset();
		gyro->Calibrate();
		gyro->Reset();

		timer->Reset();

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
		tiltArm->Execute(solenoidFire[0]);
		datafail = false;
		climbing = false;
		datafail = false;
		elevatorUp = false;
		firstDirection = false;
		secondDirection = false;
		thirdDirection = false;
		fourthDirection = false;
		timerResetter = false;
		timerResetter2 = false;
		firstRotation = false;
		secondRotation = false;
		thirdRotation = false;
		fourthDistance = false;
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
	double GyroAdjust (double straightAngle){  //not used yet need to figure out gyro directions to get proper rotation direction
		double angleDiff = straightAngle - gyro->GetAngleZ();
		if (gyro->GetAngleZ() > straightAngle +1)
			{
				gyroAdjust = (-0.025)*(angleDiff);
				//return gyroAdjust;
			}
		else if (gyro->GetAngleZ() < straightAngle -1)
			{
				gyroAdjust = (-0.025)*(angleDiff);
				//return gyroAdjust;
			}
		else
			{
				gyroAdjust = 0.0;
				//return gyroAdjust;
			}
	return gyroAdjust;
	}

	double LIDARValue (){
		if (LIDAR->Get() < 1.0){
			if (warningCount-- > 0){
				DriverStation::ReportError("Calculating distance");
			}
		}
		else {
			distance_cm = ((((LIDAR->GetPeriod() * 1000000.0)/10.0)) + LIDARCalibrationOffset);
			distance_in = distance_cm * 0.393700787;
		}
		frc::SmartDashboard::PutNumber("Distance cm",distance_cm);
		frc::SmartDashboard::PutNumber("Distance in",distance_in);
		return distance_in;
	}

	void DriveDistance(double wantedDistance){

	}

	void AutonomousPeriodic() override {
		frc::Scheduler::GetInstance()->Run();

		frc::SmartDashboard::PutNumber("selector", DSselector);
		frc::SmartDashboard::PutNumber("FMSside", FMSselector);
		robotDistance = LIDARValue();

			if (DSselector == 1) // cross auto line ONLY
			{
				DriverStation::ReportError("drive forward");
				if (timer->Get() < 3.0) // drive forward for 3 seconds then stop
				{
					piboticsDrive->Execute(0.0, 0.5, GyroAdjust(0.0), gyro->GetAngleZ());
				}
				else
				{
					piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ());
				}
			}
			if (DSselector == 3) // robot starts in middle code
			{
				DriverStation::ReportError("Driving Forward and raising elevator");
				if (timer->Get() <= 1.00) //the timer for elevator
				{
					elev->Execute(-1.0); // elevator at full speed for 1 second
				}
				else
				{
					elev->Execute(0.0);
				}
				if (timer->Get() <= 0.75) // the timer for driving forward
				{
					piboticsDrive->Execute(0.0, 0.5, GyroAdjust(0.0), gyro->GetAngleZ()); // driving forward
				}
				else
				{
					piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ()); // stopping
					firstDirection = true;
				}
				if ((FMSselector == 1 || FMSselector == 3) && firstDirection) // robot going for switch on right
				{

					if (timer->Get() <= 2.9 && timer->Get() >= 1.1) // 1.5 seconds going right
					{
						piboticsDrive->Execute(0.65, 0.0, GyroAdjust(0.0), gyro->GetAngleZ()); // going right
						DriverStation::ReportError("Driving Right"); // saying going right
					}
					/*if(timer->Get() > 5.0 && timer->Get() < 7.0)
					{
						piboticsDrive->Execute(0.0, 0.0, GyroAdjust(0.0), gyro->GetAngleZ());
					}*/
					if (timer->Get() >= 3.0 && timer->Get() <= 6.25) // going forward for the second time
							{
								DriverStation::ReportError("driving forward again"); //telling the drivers we are going forward
								piboticsDrive->Execute(0.0, 0.25, GyroAdjust(0.0), gyro->GetAngleZ()); // going forward
							}
					if(timer->Get() >= 6.25)
							{
								piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ()); // stopping
								grabber->Execute(solenoidFire[1]); //place cube in switch

							}
				}
				else if ((FMSselector == 2 || FMSselector == 4) && firstDirection) // robot going for switch on left
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
							piboticsDrive->Execute(0.0, 0.25, GyroAdjust(0.0), gyro->GetAngleZ()); // going forward
						}
					if(timer->Get() >= 6.15)
						{
							piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ()); // stopping
							grabber->Execute(solenoidFire[1]); //place cube oin switch
						}
				}
			}
			else if (DSselector == 2) // left auto (robot on left)
				{
					if (FMSselector == 1 || 4)  // left side going for scale on left side
					{
						DriverStation::ReportError("Driving Forward to LIDAR distance");
						if (robotDistance <= (distance1 - distError) && !firstDirection){ // go forward to specified distance
							piboticsDrive->Execute(0.0, 0.5, GyroAdjust(0.0), gyro->GetAngleZ());
						}
						else if (!firstDirection && robotDistance > (distance1 - distError)){ // made it to first distance
							firstDirection = true;
							piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ());
						}
						if (firstDirection && !firstRotation){ // rotate to face the scale
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(-90.0), gyro->GetAngleZ());
							elev->Execute(-1.0);
							if (gyro->GetAngleZ() <= -90.0){
								firstRotation = true;
							}
						}
						if (firstDirection && firstRotation && !secondRotation && robotDistance <= distance2){ // move forward to get close to scale
							piboticsDrive->Execute(0.0, 0.25, GyroAdjust(-90.0), gyro->GetAngleZ());
						}
						else if (!secondDirection && robotDistance > distance2){
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(-90.0), gyro->GetAngleZ());
							secondDirection = true;
						}
						if (secondDirection){ // place cube
							grabber->Execute(solenoidFire[1]);
						}
					}
					if (FMSselector == 2 || 3)  // left side going for scale on right side
					{
						DriverStation::ReportError("Driving Forward to LIDAR distance");
						if (robotDistance <= (distance3 - distError) && !firstDirection){ //move forward to area between switch and scale
							piboticsDrive->Execute(0.0, 0.5, GyroAdjust(0.0), gyro->GetAngleZ()); // going forward
						}
						else if (!firstDirection && robotDistance > (distance3 - distError)){
							firstDirection = true;
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(0.0), gyro->GetAngleZ()); // stop
						}
						if (firstDirection && !firstRotation){ //rotate robot
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(-90.0), gyro->GetAngleZ());
							if (gyro->GetAngleZ() <= -90.0){
								firstRotation = true;
							}
						}
						if (firstDirection && firstRotation && !secondRotation && robotDistance <= distance4){ // move to right side of field
							piboticsDrive->Execute(0.0, 0.50, GyroAdjust(-90.0), gyro->GetAngleZ());
						}
						else if (!secondDirection && robotDistance > distance4){
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(-90.0), gyro->GetAngleZ());
							secondDirection = true;
						}
						if (secondDirection && !secondRotation){ //rotate robot to forward face again
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(0.0), gyro->GetAngleZ());
							if (gyro->GetAngleZ() >= -0.1 && gyro->GetAngleZ() <= 0.1){
								firstRotation = true;
							}
						}
						if (secondDirection && secondRotation && robotDistance <= distance1){  // move remaining distance to line up with scale
							piboticsDrive->Execute(0.0, 0.50, GyroAdjust(0.0), gyro->GetAngleZ());
							//elev->Execute(-1.0);
						}
						else if (secondDirection && secondRotation && robotDistance > distance1){
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(0.0), gyro->GetAngleZ());
							thirdDirection = true;
						}
						if (thirdDirection && !thirdRotation){ // rotate to face scale
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(90.0), gyro->GetAngleZ());
							elev->Execute(-1.0);
							if (gyro->GetAngleZ() >= 90.0){
								thirdRotation = true;
							}
						}
						if (thirdDirection && thirdRotation && robotDistance <= distance2){ //move towards the scale
							piboticsDrive->Execute(0.0, 0.25, GyroAdjust(90.0), gyro->GetAngleZ());
						}
						else if (thirdDirection && thirdRotation && robotDistance > distance2){
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(90.0), gyro->GetAngleZ());
							fourthDistance = true;
						}
						if (fourthDistance){ // place the cube
							grabber->Execute(solenoidFire[1]);
						}
					}
				}
			else if (DSselector == 4)//right side start
				{
					if (FMSselector == 2 || 3)  // right side going for scale on right side
					{
						DriverStation::ReportError("Driving Forward to LIDAR distance");
						if (robotDistance <= (distance1 - distError) && !firstDirection){ // go forward to scale
							piboticsDrive->Execute(0.0, 0.5, GyroAdjust(0.0), gyro->GetAngleZ());
						}
						else if (!firstDirection && robotDistance > (distance1 - distError)){
							firstDirection = true;
							piboticsDrive->Execute(0.0, 0.0, 0.0, gyro->GetAngleZ()); // stop
						}
						if (firstDirection && !firstRotation){ //rotate to face the scale
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(90.0), gyro->GetAngleZ());
							elev->Execute(-1.0);
							if (gyro->GetAngleZ() >= 90.0){
								firstRotation = true;
							}
						}
						if (firstDirection && firstRotation && !secondRotation && robotDistance <= distance2){ // move closer to scale
							piboticsDrive->Execute(0.0, 0.25, GyroAdjust(90.0), gyro->GetAngleZ());
						}
						else if (!secondDirection && robotDistance > distance2){
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(90.0), gyro->GetAngleZ());
							secondDirection = true;
						}
						if (secondDirection){ // place cube
							grabber->Execute(solenoidFire[1]);
						}
					}
					if (FMSselector == 1 || 4)  // right side going for scale on left side
					{
						DriverStation::ReportError("Driving Forward to LIDAR distance");
						if (robotDistance <= (distance3 - distError) && !firstDirection){ // forward to between switch and scale
							piboticsDrive->Execute(0.0, 0.5, GyroAdjust(0.0), gyro->GetAngleZ());
						}
						else if (!firstDirection && robotDistance > (distance3 - distError)){
							firstDirection = true;
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(0.0), gyro->GetAngleZ());
						}
						if (firstDirection && !firstRotation){ //rotate robot
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(90.0), gyro->GetAngleZ());
							if (gyro->GetAngleZ() >= 90.0){
								firstRotation = true;
							}
						}
						if (firstDirection && firstRotation && !secondRotation && robotDistance <= distance4){ // drive to left side of field
							piboticsDrive->Execute(0.0, 0.50, GyroAdjust(90.0), gyro->GetAngleZ());
						}
						else if (!secondDirection && robotDistance > distance4){
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(90.0), gyro->GetAngleZ());
							secondDirection = true;
						}
						if (secondDirection && !secondRotation){ //rotate back to forward face
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(0.0), gyro->GetAngleZ());
							if (gyro->GetAngleZ() >= -0.1 && gyro->GetAngleZ() <= 0.1){
								firstRotation = true;
							}
						}
						if (secondDirection && secondRotation && robotDistance <= distance1){ // move forward to scale position
							piboticsDrive->Execute(0.0, 0.50, GyroAdjust(0.0), gyro->GetAngleZ());
							//elev->Execute(-1.0);
						}
						else if (secondDirection && secondRotation && robotDistance > distance1){
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(0.0), gyro->GetAngleZ());
							thirdDirection = true;
						}
						if (thirdDirection && !thirdRotation){ // rotate to face scale
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(-90.0), gyro->GetAngleZ());
							elev->Execute(-1.0);
							if (gyro->GetAngleZ() <= -90.0){
								thirdRotation = true;
							}
						}
						if (thirdDirection && thirdRotation && robotDistance <= distance2){ // move towards scale
							piboticsDrive->Execute(0.0, 0.25, GyroAdjust(-90.0), gyro->GetAngleZ());
						}
						else if (thirdDirection && thirdRotation && robotDistance > distance2){
							piboticsDrive->Execute(0.0, 0.0, GyroAdjust(-90.0), gyro->GetAngleZ());
							fourthDistance = true;
						}
						if (fourthDistance){ // place cube
							grabber->Execute(solenoidFire[1]);
						}
					}
				}
				else
				{
					DriverStation::ReportError("No FMS Question Field Staff");
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
		//liftEncoder->Execute(Order66[1]);

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
			//if (c_controllerCommand == &joystickChooser || c_chooser.GetSelected() == &joystickChooser)
			//{
				deadZone();
				//piboticsDrive->Execute(xin, -yin, zin, gyro->GetAngleZ());//was negative gyro not needed
			//} not needed
			//else not needed
			//{ not needed
				//ControllerNerf();

				robotDistance = LIDARValue();


				frc::SmartDashboard::PutNumber("robot dist inch", robotDistance);

				if (driveJoy->GetRawButton(1))
				{
					piboticsDrive->Execute(xin, -yin, zin, 0.0);
				}
				else
				{
					piboticsDrive->Execute(xin, -yin, zin, gyro->GetAngleZ()); // REMOVED THE NEGATIVE GYRO
				}
			//}

			if (driveJoy->GetRawButton(7) || driveCont->GetRawButton(7))
					{
						gyro->Reset();
					}

			//Motor Stuff
			//run winch
			if ((buttonJoy->GetRawButton(1) && buttonJoy->GetRawButton(2)) || (buttonPad->GetRawButton(4) && buttonPad->GetRawButton(5)))
			{
				winch->Execute();
			}
			else
			{
				winch->End();
			}

			//tilt cube
			if (buttonJoy2->GetRawButton(10) || buttonJoy->GetRawButton(8)||buttonCont->GetRawButton(2))
			{
				tiltArm->Execute(solenoidFire[0]);
			}
			//untilt the cube
			if (buttonJoy2->GetRawButton(9) || buttonJoy->GetRawButton(7)||buttonCont->GetRawButton(3))
			{
				tiltArm->Execute(solenoidFire[1]);
			}

			//elevator down
			if (buttonJoy2->GetRawButton(4)||buttonJoy->GetRawButton(9)||buttonCont->GetRawButton(1))//nerfed so we dont derail XV
			{
				elev->Execute(1.0);
			}
			//elevator up
			else if (buttonJoy2->GetRawButton(6)||buttonJoy->GetRawButton(10)||buttonCont->GetRawButton(4))
			{
				elev->Execute(-1.0);
			}
			else
			{
				elev->End();
			}

			//Pneumatics stuff
			//close grabbers
			if (buttonJoy2->GetRawButton(12) || buttonJoy->GetRawButton(5)||buttonCont->GetRawButton(6))
			{
				grabber->Execute(solenoidFire[0]);
			}
			//open grabbers
			if (buttonJoy->GetRawButton(11) || buttonJoy->GetRawButton(6)||buttonCont->GetRawButton(5))
			{
				grabber->Execute(solenoidFire[1]);
			}

			//tilt the robot
			if ((buttonJoy->GetRawButton(3) && buttonJoy->GetRawButton(4)) || (buttonPad->GetRawButton(3) && buttonPad->GetRawButton(6)))
			{
				tiltRobot->Execute(solenoidFire[0]);
			}
			//untilt the robot
			if (buttonPad->GetRawButton(1))
			{
				tiltRobot->Execute(solenoidFire[1]);
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
				tiltRobot->Execute(solenoidFire[1]);
				cTime = 0.0;
				timer->Stop();
				timer->Reset();
				timerResetter = false;
				climbing = false;
			}


	}


	void TestPeriodic() override {

		frc::SmartDashboard::PutNumber("GyroAngle", gyro->GetAngleZ());
		timer->Start();
		if (timer->Get() < 5.0){

			piboticsDrive->Execute(0.0,0.0,GyroAdjust(-90.0),gyro->GetAngleZ());

		}
		else
			piboticsDrive->Execute(0.0,0.0,GyroAdjust(45.0),gyro->GetAngleZ());
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
