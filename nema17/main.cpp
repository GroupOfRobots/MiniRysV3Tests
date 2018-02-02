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
	if (argc < 3) {
		std::cout << "[ERROR] Please pass enough arguments -> (speed, step). Aborting..." << std::endl;
		return 0;
	}
	signal(SIGINT, sigintHandler);

	std::cout << "[MOTORS] Initializing motors..." << std::endl;
	MotorsController * motorsController = new MotorsController();
	std::cout << "[MOTORS] Enabling motors..." << std::endl;
	motorsController->enableMotors();
	motorsController->setInvertSpeed(true, false);
	std::cout << "[MOTORS] Running..." << std::endl;

	float speed = atof(argv[1]);
	int step = atoi(argv[2]);

	while(!exitFlag) {

		motorsController->setMotorSpeeds(0.01f, speed, step, true);
		std::cout << motorsController->getMotorSpeedRightRaw() << std::endl;
		usleep(200 * 1000);

	}

	std::cout << "\n[MOTORS] Deleting MotorsController and exiting..." << std::endl;
	delete motorsController;
	return 0;

}
