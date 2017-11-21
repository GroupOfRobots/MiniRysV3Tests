all: bin/mpu6050

bin/mpu6050: mpu6050/I2Cdev.cpp mpu6050/IMU.cpp mpu6050/MPU6050.cpp mpu6050/main.cpp
	g++ mpu6050/I2Cdev.cpp mpu6050/IMU.cpp mpu6050/MPU6050.cpp mpu6050/main.cpp -o bin/mpu6050

clean:
	rm -rf *.o
	rm -rf mpu6050

.PHONY: all clean
