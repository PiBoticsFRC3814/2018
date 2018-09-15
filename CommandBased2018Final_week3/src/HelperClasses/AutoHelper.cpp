/*
 * AutoHelper.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: PiBotics
 */

#include <HelperClasses/AutoHelper.h>


AutoHelper::AutoHelper() {
	// TODO Auto-generated constructor stub


}

AutoHelper::~AutoHelper() {
	// TODO Auto-generated destructor stub
}

double AutoHelper::inchesToEnc(double inches)
{

return (inches/rollCir)*encCountPerRev;
}
