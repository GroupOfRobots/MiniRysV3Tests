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

	IMU * imu = new IMU();

	while(!exitFlag) {
		IMU::ImuData data;
		// int result;
		try {
			imu->getData(&data);
		} catch (std::string & error) {
			std::cout << "[IMU] Error getting IMU reading: " << error << std::endl;
			continue;
		}

		// if (result < 0) {
		// 	continue;
		// }

		// std::cout << std::endl;
	}

	delete imu;
	return 0;
}
