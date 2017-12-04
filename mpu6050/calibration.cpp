#include <iostream>
#include <iomanip>
#include <csignal>
#include <thread>
#include "MPU6050.hpp"

bool exitFlag = false;
bool accumulateDataFlag = false;
int64_t axs = 0;
int64_t ays = 0;
int64_t azs = 0;
int64_t gxs = 0;
int64_t gys = 0;
int64_t gzs = 0;
uint32_t samples = 0;

void sigintHandler(int signum) {
	if (signum == SIGINT) {
		exitFlag = true;
	}
}

void imuThreadFn() {
	std::cout << "initializing imu\n";
	MPU6050 * imu = new MPU6050();
	imu->initialize();
	if (!imu->testConnection()) {
		throw(std::string("MPU6050 connection failed"));
	}

	std::cout << "reading\n";

	int16_t ax, ay, az, gx, gy, gz;

	while(!exitFlag) {
		try {
			imu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		} catch (std::exception & e) {
			std::cout << "[IMU] Error getting IMU reading: " << e.what() << std::endl;
			usleep(50 * 1000);
			continue;
		}

		// std::cout << std::fill(6);
		std::cout << ax << " | " << ay << " | " << az << " | " << gx << " | " << gy << " | " << gz << std::endl;

		if (accumulateDataFlag) {
			axs += ax;
			ays += ay;
			azs += az;
			gxs += gx;
			gys += gy;
			gzs += gz;
			samples++;
		}

		usleep(50 * 1000);
	}

	delete imu;
}

int main(int argc, char * argv[]) {
	signal(SIGINT, sigintHandler);

	std::cout << "[ENTER] to start gathering data";
	std::thread imuThread(imuThreadFn);
	std::cin.get();
	std::cout << "[ENTER] to stop gathering data";
	std::cin.get();
	exitFlag = true;
	imuThread.join();

	std::cout << "Average reads:\n";
	std::cout << "Acceleration:\n";
	std::cout << "\tAX: " << axs/samples << " | AY: " << ays/samples << " | AZ: " << azs/samples << std::endl;
	std::cout << "Rotation (gyro):\n";
	std::cout << "\tGX: " << gxs/samples << " | GY: " << gys/samples << " | GZ: " << gzs/samples << std::endl;

	return 0;
}
