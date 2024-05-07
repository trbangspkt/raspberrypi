#include <stdio.h>   
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "../I2Cdev_MPU6050/MPU6050.h"
#include "../SPIdev_MAX7219/MAX7219.h"

long timer = 0;

int main(void) {

    loadI2C(); /* load I2C device */ 
    initmpu(); /* set up mpu6050 */   
    loadSPI(); /* load SPI device */ 
    initMax7219(); /* set up max7219 */  
    system("clear"); 
    displayTemp(0.0);
    // MPU6050_calcGyroOffsets(); /* Caculate GyroOffsets for 1st */
    MPU6050_setGyroOffsets(11.256861, 2.518711, 1.223424); /* Set GyroOffsets */

    initMillis();

    while (1) {
        MPU6050_update();

        display1Angle(getAngleZ()); /* Display angle Z on LED*/
        // display3Number(getAngleX(), getAngleY(), getAngleZ()); /* Display 3 angle Z on LED*/

        if(millis() - timer > 1000){

            // displayTemp(getTemp());  /* Display temperature on LED*/
            
            system("clear"); 
            printf("Gyro Z Offsets: %f\n", getGyroZoffset());
            printf("=======================================================\n");
            printf("temp : %.2f\n\n", getTemp());
            printf("accX : %.2f\n", getAccX());
            printf("accY : %.2f\n", getAccY());
            printf("accZ : %.2f\n\n", getAccZ());
              
            printf("gyroX : %.2f\n", getGyroX());
            printf("gyroY : %.2f\n", getGyroY());
            printf("gyroZ : %.2f\n\n", getGyroZ());
              
            printf("accAngleX : %.2f\n", getAccAngleX());
            printf("accAngleY : %.2f\n\n", getAccAngleX());
            
            printf("gyroAngleX : %.2f\n", getGyroAngleX());
            printf("gyroAngleY : %.2f\n", getGyroAngleY());
            printf("gyroAngleZ : %.2f\n\n", getGyroAngleZ());
                  
            printf("angleX : %.2f\n", getAngleX());
            printf("angleY : %.2f\n", getAngleY());
            printf("angleZ : %.2f\n", getAngleZ());
            printf("=======================================================\n");

            timer = millis();
        }
    }
    return 0;
}
