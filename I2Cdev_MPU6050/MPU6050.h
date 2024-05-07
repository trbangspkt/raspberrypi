#define Sample_rate     25
#define Config          26
#define Gyro_config     27
#define Acc_config      28
#define Interrupt       56
#define PWR_Managment   107 

#define Temperature 65
#define Acc_X  59
#define Acc_Y  61
#define Acc_Z  63
#define Gyro_X 67
#define Gyro_Y 69
#define Gyro_Z 71

void sendDataI2C(uint8_t address, uint8_t value);
int readDataI2C(uint8_t addr); 
void loadI2C(void);
void initmpu(void);
int16_t read_sensor(unsigned char sensor);
void initMillis();
unsigned int millis(void);
void MPU6050_setGyroOffsets(float x, float y, float z);
void MPU6050_calcGyroOffsets(void);
void MPU6050_update(void);

float getTemp();
float getAccX();
float getAccY();
float getAccZ();

float getGyroX();
float getGyroY();
float getGyroZ();

float getGyroXoffset();
float getGyroYoffset();
float getGyroZoffset();

float getAccAngleX();
float getAccAngleY();

float getGyroAngleX();
float getGyroAngleY();
float getGyroAngleZ();

float getAngleX();
float getAngleY();
float getAngleZ();
