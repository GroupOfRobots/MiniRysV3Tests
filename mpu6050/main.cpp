#include <iostream>
#include <csignal>
#include "IMU.hpp"

bool exitFlag = false;

void sigintHandler(int signum) {
	if (signum == SIGINT) {
		exitFlag = true;
	}
}

int main(int argc, char * argv[]) {
	signal(SIGINT, sigintHandler);

	std::cout << "initializing imu\n";
	IMU * imu = new IMU();
	imu->initialize();

	std::cout << "reading\n";

	while(!exitFlag) {
		IMU::ImuData data;
		try {
			imu->getData(&data);
		} catch (std::string & error) {
			std::cout << "[IMU] Error getting IMU reading: " << error << std::endl;
			continue;
		}

		usleep(50 * 1000);
	}

	delete imu;
	return 0;
}
