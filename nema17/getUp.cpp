#include <iostream>
#include <unistd.h>
#include <csignal>
#include "MotorsController.hpp"

#define SLEEPY_TIME 20000

bool exitFlag = false;

void sigintHandler(int signum) {
	if (signum == SIGINT) {
		exitFlag = true;
	}
}

int main(int argc, char * argv[]) {
	signal(SIGINT, sigintHandler);

	float speed_mod = 1.0;
	if (argc > 1 and argv[1] == std::string("-r")) {
		speed_mod = -1.0;
	}

	std::cout << "[MOTORS] Initializing motors..." << std::endl;
	MotorsController * motorsController = new MotorsController();
	std::cout << "[MOTORS] Enabling motors..." << std::endl;
	motorsController->enableMotors();
	motorsController->setInvertSpeed(true, false);
	motorsController->setMotorsSwapped(true);
	std::cout << "[MOTORS] Running..." << std::endl;

	while(!exitFlag) {

		for(int i=0; i<12 and !exitFlag; i++)
		{
			motorsController->setMotorSpeeds(speed_mod*1.0, speed_mod*1.0, 32, false);
			std::cout << motorsController->getMotorSpeedLeft() << ';' << motorsController->getMotorSpeedRight() << std::endl;
			usleep(SLEEPY_TIME);
		}
		for(int i=0; i<15 and !exitFlag; i++)
		{
			motorsController->setMotorSpeeds(speed_mod*(-1.0), speed_mod*(-1.0), 32, false);
			std::cout << motorsController->getMotorSpeedLeft() << ';' << motorsController->getMotorSpeedRight() << std::endl;
			usleep(SLEEPY_TIME);
		}
		speed_mod = -speed_mod;
		usleep(SLEEPY_TIME*200);

	}

	std::cout << "\n[MOTORS] Deleting MotorsController and exiting..." << std::endl;
	delete motorsController;
	return 0;

}
