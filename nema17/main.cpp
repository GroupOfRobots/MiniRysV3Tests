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
	std::cout << "[MOTORS] Running..." << std::endl;

	while(!exitFlag) {

		motorsController->setMotorSpeeds(0, 0, 1, true);
		std::cout << motorsController->getMotorSpeedRightRaw() << std::endl;
		usleep(200 * 1000);

	}

	std::coud "\n[MOTORS] Deleting MotorsController and exiting..." << std::endl;
	delete motorsController;
	return 0;

}
