CFLAGS := -Wall -Werror

MPU6050_SRCS := mpu6050/I2Cdev.o mpu6050/IMU.o mpu6050/MPU6050.o mpu6050/main.o
MPU6050_OBJS := ${MPU6050_SRCS:.cpp=.o}

all: bin/mpu6050

bin/mpu6050: ${MPU6050_OBJS}
	g++ ${CFLAGS} ${MPU6050_OBJS} -o bin/mpu6050

%.o: %.cpp Makefile
	g++ ${CFLAGS} -c $< -o $@

clean:
	rm -rf MPU6050_OBJS
	rm -rf mpu6050

.PHONY: all clean
