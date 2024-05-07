# Complide - Truong Thanh Bang
CC = gcc

AR = ar

build:
	$(CC) -c ../I2Cdev_MPU6050/MPU6050.c -o MPU6050.o -lm -li2c
	$(CC) -c ../SPIdev_MAX7219/MAX7219.c -o MAX7219.o

create:
	$(AR) rc libMPU6050.a MPU6050.o
	$(AR) rc libMAX7219.a MAX7219.o
