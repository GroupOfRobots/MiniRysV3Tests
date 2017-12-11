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
	if (argc < 2) {
		std::cout << "[ERROR] Please set right motor speed with argument. Aborting..." << std::endl;
		return 0;
	}
	signal(SIGINT, sigintHandler);

	std::cout << "[MOTORS] Initializing motors..." << std::endl;
	MotorsController * motorsController = new MotorsController();
	std::cout << "[MOTORS] Enabling motors..." << std::endl;
	motorsController->enableMotors();
	std::cout << "[MOTORS] Running..." << std::endl;

	while(!exitFlag) {

		motorsController->setMotorSpeeds(0, argv[1], 1, true);
		std::cout << motorsController->getMotorSpeedRightRaw() << std::endl;
		usleep(200 * 1000);

	}

	std::cout << "\n[MOTORS] Deleting MotorsController and exiting..." << std::endl;
	delete motorsController;
	return 0;

}
