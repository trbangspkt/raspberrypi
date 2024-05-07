#include <stdio.h>   
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>  
#include <time.h>      
#include <linux/i2c-dev.h>  
#include <i2c/smbus.h>      
#include <fcntl.h>                       
#include <sys/ioctl.h>          
#include "MPU6050.h"

// Global Parameters for get data from MPU6050
float gyroXoffset, gyroYoffset, gyroZoffset, temp, accX, accY, accZ, gyroX, gyroY, gyroZ;
float angleAccX, angleAccY, angleGyroX, angleGyroY, angleGyroZ, angleX, angleY, angleZ;
float accCoef = 0.02, gyroCoef = 0.98;
float interval;
unsigned long preInterval;

// I2C parameters 
int address = 0x68; /* The I2C adress*/
int mpu;

// Send Data I2C 
void sendDataI2C(uint8_t address, uint8_t value) {
    i2c_smbus_write_word_data(mpu, address, value);
}
// Read Data I2C
int readDataI2C(uint8_t addr) {
    int32_t res = i2c_smbus_read_word_data(mpu, addr);
    if (res < 0) {
        /*ERROR HANDLING; you can check errno to see what went wrong*/
    } else {
        return res & 0xFF;
    }
}
// Load I2C 
void loadI2C(void) {
    // Load I2C driver
    mpu = open("/dev/i2c-1", O_RDWR);
    if (mpu < 0) {
        printf("Can't load I2C driver\n");
        exit(1);
    }
    // Set Slave adress
    if (ioctl(mpu, I2C_SLAVE, address) < 0) {
        printf("Can't connect to MPU6050\n");
        exit(1);
    }
    // Check I2C connection 
    if (readDataI2C(117) != 0x68){
        printf("Connection Fail \n");
        exit(1);
    }
}
// Init 6050 I2C
void initmpu(void) {
    // Set MPU6050 mode
    sendDataI2C(Sample_rate, 1);        // sample rate = 500Hz
    sendDataI2C(Config, 3);             // DLPF on, fc =44Hz
    sendDataI2C(Gyro_config, 0x00);     // +- 250 degree/s
    sendDataI2C(Acc_config, 0x00);      // +- 2g
    sendDataI2C(PWR_Managment, 0x01);   // PLL with X axis gyroscope reference
}
// Read register of MPU
int16_t read_sensor(unsigned char sensor) {
    int16_t high, low, data;
    high = readDataI2C(sensor);
    low = readDataI2C(sensor+1);
    data = (high<<8)|low;
    return data;
}
// Stores the initialization time
static uint64_t epochMilli;
// Init epoch time
void initMillis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    epochMilli = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L);
}

// Returns the number of milliseconds since epoch
unsigned int millis(void) {
    uint64_t now;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    now = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L);

    return (uint32_t)(now - epochMilli);
}

void MPU6050_setGyroOffsets(float x, float y, float z){
  gyroXoffset = x;
  gyroYoffset = y;
  gyroZoffset = z;
}

void MPU6050_calcGyroOffsets(void){
    float x = 0, y = 0, z = 0;
	float rx, ry, rz;
    usleep(500000);
    printf("========================================\n");
    printf("Calculating gyro offsets\n");
    printf("DO NOT MOVE MPU6050\n");
    for(int i = 0; i < 100; i++){
        // Gyroscope Measurements
        rx = (float)read_sensor(Gyro_X)/131.0;
        ry = (float)read_sensor(Gyro_Y)/131.0;
        rz = (float)read_sensor(Gyro_Z)/131.0;
        x += rx;
        y += ry;
        z += rz;
    }
    gyroXoffset = x / 100;
    gyroYoffset = y / 100;
    gyroZoffset = z / 100;
    printf("Done!\n");
    printf("X : %f\n", gyroXoffset);
    printf("Y : %f\n", gyroYoffset);
    printf("Z : %f\n", gyroZoffset);
    printf("Program will start after 1 seconds\n");
    printf("========================================\n");
    usleep(1000000);
}

void MPU6050_update(void){
    // Read Sensor Temperature
    temp = ((float)read_sensor(Temperature))/340.0 + 36.53;
    // Accerlerometer Measurements
    accX = ((float)read_sensor(Acc_X))/16384.0;
    accY = ((float)read_sensor(Acc_Y))/16384.0;
    accZ = ((float)read_sensor(Acc_Z))/16384.0;
    // Gyroscope Measurements
    gyroX = ((float)read_sensor(Gyro_X))/131.0;
    gyroY = ((float)read_sensor(Gyro_Y))/131.0;
    gyroZ = ((float)read_sensor(Gyro_Z))/131.0;
    // Roll, Pitch (Accerlerometer Measurements)
    angleAccX = atan2(accY, sqrt(pow(accZ, 2) + pow(accX, 2))) * 180 / M_PI;
    angleAccY = atan2(accX, sqrt(pow(accZ, 2) + pow(accY, 2))) * -180 / M_PI;
    // Fix gyro
    gyroX -= gyroXoffset;
    gyroY -= gyroYoffset;
    gyroZ -= gyroZoffset;
    // Sampling times for calculatation
    interval = (float)(millis() - preInterval) * 0.001;
    // Roll, Pitch, Yaw (Gyroscope Measurements)
    angleGyroX += gyroX * interval;
    angleGyroY += gyroY * interval;
    angleGyroZ += gyroZ * interval;
    // Roll, Pitch, Yaw 
    angleX = (gyroCoef * (angleX + gyroX * interval)) + (accCoef * angleAccX);
    angleY = (gyroCoef * (angleY + gyroY * interval)) + (accCoef * angleAccY);
    angleZ = angleGyroZ;
    preInterval = millis();
}
float getTemp(){ return temp; }
float getAccX(){ return accX; }
float getAccY(){ return accY; }
float getAccZ(){ return accZ; }

float getGyroX(){ return gyroX; }
float getGyroY(){ return gyroY; }
float getGyroZ(){ return gyroZ; }

float getGyroXoffset(){ return gyroXoffset; }
float getGyroYoffset(){ return gyroYoffset; }
float getGyroZoffset(){ return gyroZoffset; }

float getAccAngleX(){ return angleAccX; }
float getAccAngleY(){ return angleAccY; }

float getGyroAngleX(){ return angleGyroX; }
float getGyroAngleY(){ return angleGyroY; }
float getGyroAngleZ(){ return angleGyroZ; }

float getAngleX(){ return angleX; }
float getAngleY(){ return angleY; }
float getAngleZ(){ return angleZ; }
