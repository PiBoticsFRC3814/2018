/*
 * Auto_4.h
 *
 *  Created on: Feb 14, 2018
 *      Author: PiBotics
 */

#ifndef SRC_COMMANDS_AUTO_4_H_
#define SRC_COMMANDS_AUTO_4_H_

#include <Commands/Command.h>
#include <Commands/PiBoticsDrive.h>

class Auto_4 : public frc::Command {

public:
	Auto_4();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif/* SRC_COMMANDS_AUTO_4_H_ */
