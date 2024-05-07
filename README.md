# Code MPU6050 and MAX7219
Embedded System at HCMUTE

## Hardware connection
 
|   RASPBERRYPI PIN    |   MPU6050 PIN   |   |   RASPBERRYPI PIN   |   MAX7219 PIN   |
| :-------------------:|:---------------:|:-:|:-------------------:|:---------------:|
|        1 (3.3V)      |       VCC       |   |         2 (5V)      |       VCC       |
|           6          |       GND       |   |          20         |       GND       |
|           3          |       SDA       |   |          19         |       DIN       |
|           5          |       SCL       |   |          24         |        CS       |
|                      |                 |   |          23         |       CLK       |  

## Wiring diagram raspberry with MPU6050 and max7219
![Image.](/pic/Connection.png)

## Demo
![demo.](/pic/demo.jpg)

## Compile and run the program
Download all file to the same path on raspberry.

```
  cd path

  # Compile
  make
  # or
  make build
  # or 
  gcc example/getAllData.c -L. ./lib/libMPU6050.a -L. ./lib/libMAX7219.a -lm -li2c -o run

  # Run
  ./run
```
