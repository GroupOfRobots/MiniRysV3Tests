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

	std::cout << "[MOTORS] Initializing motors...\n";
	MotorsController * motorsController = new MotorsController();
	motorsController->disableMotors();

	while(!exitFlag) {

		usleep(50 * 1000);

	}

	delete motorsController;
	return 0;

}