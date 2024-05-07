# Complide - Truong Thanh Bang
SOURCE_CODE = ./example/getAllData.c
CC =  gcc

STA_LIB = ./lib/libMPU6050.a ./lib/libMAX7219.a 

SOURCE_LIB = -lm -li2c

$(foreach lib,$(STA_LIB),$(eval SOURCE_STA_LIB += -L. $(lib)))

build: $(SOURCE_CODE)
	$(CC) $? $(SOURCE_STA_LIB) $(SOURCE_LIB) -o run
