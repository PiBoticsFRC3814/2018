/*
 * AutoHelper.h
 *
 *  Created on: Feb 24, 2018
 *      Author: PiBotics
 *      This is a helper class for the autonomous modes, it calculates encoder values
 *      and converts to field distances(in feet) for readability in code
 */

#ifndef SRC_HELPERCLASSES_AUTOHELPER_H_
#define SRC_HELPERCLASSES_AUTOHELPER_H_

#include <Commands/PiBoticsDrive.h>
#include <SmartDashboard/SmartDashboard.h>
#include "ADIS16448_IMU.h"
#include "ctre/Phoenix.h"
#include "Commands/Elevator.h"
#include "Commands/Actuate.h"

class AutoHelper {
public:
	constexpr static double PI = 3.1415926;
	constexpr static double rollCir =PI*4;
	constexpr static double encCountPerRev =8192.0;
	constexpr static double autoLine = 120.0;

	/* Actual encoder counts to be used in auto.
	 * Converts from distance values in inches to
	 * the encoder count.
	 * Returns absolute value because robot
	 * never backtracks during auto
	 * programs.
	 *
	 */
	double dist1 = abs(inchesToEnc(D1));
	double dist2 = abs(inchesToEnc(D2));
	double dist3 = abs(inchesToEnc(D3));
	double dist4 = abs(inchesToEnc(D4));
	double dist5 = abs(inchesToEnc(D5));
	double dist6 = abs(inchesToEnc(D6));
	double dist7 = abs(inchesToEnc(D7));

	const double middleSwtich[3] = {dist1,dist2,dist3};
	const double sideSwtich[4] = {dist4,dist5,dist6,dist7};

	AutoHelper();
	double inchesToEnc(double inchDistance);
	virtual ~AutoHelper();

	//Actuate* autoGrabber = new Actuate(2,3);
	//Elevator* autoElev  = new Elevator();

	bool elevUp = false;
	bool cubeReleased = false;
	bool runOnce = true;
	//Timer* time;

private:
	constexpr static double D1 = 12.0; //middle pos, fwd
	constexpr static double D2 = 60.0; //middle pos, left/right
	constexpr static double D3 = 95.0; //middle pos,final fwd
	constexpr static double D4 = 194.0; //left/right pos, fwd //previously 212
	constexpr static double D5 = 12.0;  //left/right pos, left/right //previously 28.0
	constexpr static double D6 = 183.0; //left/right pos, left/right
	constexpr static double D7 = 43.0; //left/right pos, fwd  //previously 67.0

};

#endif /* SRC_HELPERCLASSES_AUTOHELPER_H_ */
