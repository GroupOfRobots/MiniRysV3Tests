#include <iostream>
#include <unistd.h>
#include <csignal>
#include "MotorsController.hpp"

bool exitFlag = false;

void sigintHandler(int signum) {
	if (signum == SIGINT) {
		exitFlag = true;
	}
}

int main(int argc, char * argv[]) {
	signal(SIGINT, sigintHandler);

	std::cout << "[MOTORS] Initializing motors..." << std::endl;
	MotorsController * motorsController = new MotorsController();
	std::cout << "[MOTORS] Enabling motors..." << std::endl;
	motorsController->enableMotors();
	motorsController->setInvertSpeed(true, false);
	motorsController->setMotorsSwapped(true);
	std::cout << "[MOTORS] Running..." << std::endl;

	while(!exitFlag) {

		motorsController->setMotorSpeeds(1.0, 1.0, 32, false);
		std::cout << motorsController->getMotorSpeedLeft() << ';' << motorsController->getMotorSpeedRight() << std::endl;
		usleep(200 * 1000);

	}

	std::cout << "\n[MOTORS] Deleting MotorsController and exiting..." << std::endl;
	delete motorsController;
	return 0;

}
