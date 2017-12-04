CFLAGS := -Wall -Wextra

MPU6050_SRCS := mpu6050/I2Cdev.o mpu6050/IMU.o mpu6050/MPU6050.o
MPU6050_OBJS := ${MPU6050_SRCS:.cpp=.o}

all: bin/mpu6050 bin/calibration

bin/mpu6050: ${MPU6050_OBJS} mpu6050/main.o
	g++ ${CFLAGS} ${MPU6050_OBJS} mpu6050/main.o -o bin/mpu6050

bin/calibration: ${MPU6050_OBJS} mpu6050/calibration.o
	g++ ${CFLAGS} ${MPU6050_OBJS} mpu6050/calibration.o -o bin/calibration -lpthread

%.o: %.cpp Makefile
	g++ ${CFLAGS} -c $< -o $@

clean:
	rm -rf mpu6050/*.o
	rm -rf bin/*

.PHONY: all clean
