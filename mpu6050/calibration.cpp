#include <iostream>
#include <iomanip>
#include <csignal>
#include <thread>
#include "MPU6050.hpp"

bool exitFlag = false;
bool accumulateDataFlag = false;
double sums[6] = {0};
double samples = 0;
int offsets[6] = {0};

void sigintHandler(int signum) {
	if (signum == SIGINT) {
		exitFlag = true;
	}
}

void parseOffsets(int argc, char * argv[]) {
	if (argc < 7) {
		return;
	}
	for (int i = 0; i < 6; ++i) {
		offsets[i] = std::stoi(argv[i+1]);
	}
}

void imuThreadFn() {
	std::cout << "initializing imu\n";
	MPU6050 * imu = new MPU6050();
	imu->initialize();
	if (!imu->testConnection()) {
		throw(std::string("MPU6050 connection failed"));
	}

	imu->setXAccelOffset(-offsets[0]/4);
	imu->setYAccelOffset(-offsets[1]/4);
	imu->setZAccelOffset((16384-offsets[2])/4);
	imu->setXGyroOffset(-offsets[3]/4);
	imu->setYGyroOffset(-offsets[4]/4);
	imu->setZGyroOffset(-offsets[5]/4);

	std::cout << "reading\n";

	int16_t data[6];

	while(!exitFlag) {
		try {
			imu->getMotion6(&data[0], &data[1], &data[2], &data[3], &data[4], &data[5]);
		} catch (std::exception & e) {
			std::cout << "[IMU] Error getting IMU reading: " << e.what() << std::endl;
			usleep(50 * 1000);
			continue;
		}

		// std::cout << std::fill(6);
		std::cout << data[0] << " | " << data[1] << " | " << data[2] << " | " << data[3] << " | " << data[4] << " | " << data[5] << std::endl;

		if (accumulateDataFlag) {
			for (int i = 0; i < 6; ++i) {
				sums[i] += data[i];
			}
			samples++;
		}

		usleep(50 * 1000);
	}

	delete imu;
}

int main(int argc, char * argv[]) {
	parseOffsets(argc, argv);
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
	std::cout << "\tAX: " << sums[0]/samples << " | AY: " << sums[1]/samples << " | AZ: " << sums[2]/samples << std::endl;
	std::cout << "Rotation (gyro):\n";
	std::cout << "\tGX: " << sums[3]/samples << " | GY: " << sums[4]/samples << " | GZ: " << sums[5]/samples << std::endl;

	std::cout << "1line:\n";
	for (int i = 0; i < 6; ++i) {
		sums[i] += (sums[i] >= 0 ? 0.5 : -0.5);
		std::cout << static_cast<int>(sums[i]) << " ";
	}
	std::cout << std::endl;

	return 0;
}
