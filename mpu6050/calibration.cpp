#include <iostream>
#include <iomanip>
#include <csignal>
#include <thread>
#include "MPU6050.hpp"

bool exitFlag = false;
bool accumulateDataFlag = false;
double axs = 0;
double ays = 0;
double azs = 0;
double gxs = 0;
double gys = 0;
double gzs = 0;
double samples = 0;

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
	imu->setXAccelOffset(0);
	imu->setYAccelOffset(0);
	imu->setZAccelOffset(0);
	imu->setXGyroOffset(0);
	imu->setYGyroOffset(0);
	imu->setZGyroOffset(0);

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

	std::cout << "[ENTER] to start gathering data\n";
	std::thread imuThread(imuThreadFn);
	std::cin.get();
	accumulateDataFlag = true;
	std::cout << "[ENTER] to stop gathering data\n";
	std::cin.get();
	accumulateDataFlag = false;
	exitFlag = true;
	imuThread.join();

	std::cout << "Average reads in " << samples << " samples:\n";
	std::cout << "Acceleration:\n";
	std::cout << "\tAX: " << axs/samples << " | AY: " << ays/samples << " | AZ: " << azs/samples << std::endl;
	std::cout << "Rotation (gyro):\n";
	std::cout << "\tGX: " << gxs/samples << " | GY: " << gys/samples << " | GZ: " << gzs/samples << std::endl;

	return 0;
}
