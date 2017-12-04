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

	imu->setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
	imu->setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

	std::cout << "Setting offsets:";
	for (int i = 0; i < 6; ++i) {
		std::cout << " " << offsets[i];
	}
	std::cout << std::endl;

	imu->setXAccelOffset(-offsets[0]);
	imu->setYAccelOffset(-offsets[1]);
	imu->setZAccelOffset(-offsets[2] + 2048);
	imu->setXGyroOffset(-offsets[3]);
	imu->setYGyroOffset(-offsets[4]);
	imu->setZGyroOffset(-offsets[5]);

	std::cout << "reading\n";

	int32_t data[6];
	int16_t ax, ay, az, gx, gy, gz;

	while(!exitFlag) {
		try {
			// imu->getMotion6(&data[0], &data[1], &data[2], &data[3], &data[4], &data[5]);
			imu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
			data[0] = ax;
			data[1] = ay;
			data[2] = az;
			data[3] = gx;
			data[4] = gy;
			data[5] = gz;
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
	if (exitFlag) {
		imuThread.join();
		return 0;
	}
	accumulateDataFlag = true;
	std::cout << "[ENTER] to stop gathering data\n";
	std::cin.get();
	if (exitFlag) {
		imuThread.join();
		return 0;
	}
	accumulateDataFlag = false;
	exitFlag = true;
	imuThread.join();

	std::cout << "Average reads in " << samples << " samples:\n";
	std::cout << "Acceleration:\n";
	std::cout << "\tAX: " << sums[0]/samples << " | AY: " << sums[1]/samples << " | AZ: " << sums[2]/samples << std::endl;
	std::cout << "Rotation (gyro):\n";
	std::cout << "\tGX: " << sums[3]/samples << " | GY: " << sums[4]/samples << " | GZ: " << sums[5]/samples << std::endl;

	std::cout << "Averages:\n";
	for (int i = 0; i < 6; ++i) {
		std::cout << static_cast<int>(sums[i]/samples) << " ";
	}
	std::cout << "\navg+offset:\n";
	for (int i = 0; i < 6; ++i) {
		std::cout << static_cast<int>(sums[i]/samples) + offsets[i] + (i == 2 ? -2048 : 0) << " ";
	}
	std::cout << std::endl;

	return 0;
}
