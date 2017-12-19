CFLAGS := -Wall -Wextra

MPU6050_SRCS := mpu6050/I2Cdev.cpp mpu6050/IMU.cpp mpu6050/MPU6050.cpp
MPU6050_OBJS := ${MPU6050_SRCS:.cpp=.o}

NEMA17_SRCS := nema17/MotorsController.cpp
NEMA17_OBJS := ${NEMA17_SRCS:.cpp=.o}

all: bin/mpu6050 bin/calibration

bin/mpu6050: ${MPU6050_OBJS} mpu6050/main.o
	g++ ${CFLAGS} ${MPU6050_OBJS} mpu6050/main.o -o bin/mpu6050

bin/calibration: ${MPU6050_OBJS} mpu6050/calibration.o
	g++ ${CFLAGS} ${MPU6050_OBJS} mpu6050/calibration.o -o bin/calibration -lpthread

bin/nema17: ${NEMA17_OBJS} nema17/main.o
	g++ ${CFLAGS} ${NEMA17_OBJS} nema17/main.o -o bin/nema17

bin/odometrySpeed: ${NEMA17_OBJS} nema17/odometrySpeed.o
	g++ ${CFLAGS} ${NEMA17_OBJS} nema17/odometrySpeed.o -o bin/odometrySpeed

%.o: %.cpp %.hpp Makefile
	g++ ${CFLAGS} -c $< -o $@

%.o: %.cpp Makefile
	g++ ${CFLAGS} -c $< -o $@

clean:
	rm -rf mpu6050/*.o
	rm -rf nema17/*.o
	rm -rf bin/*

.PHONY: all clean
