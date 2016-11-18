#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<fcntl.h>
#include"SPIDevice.h"
#include"ADXL345.h"

int main(){
	char* registerdata;
	SPIDevice spidevice;
	int i;
	openDevice(&spidevice);
	spi_writeRegister(&spidevice,0x2D,0x08);
	spi_writeRegister(&spidevice,0x31,0x08);
	registerdata=readRegisters(&spidevice,0x40,0x00);
	/*for(i=0;i<0x40;i++){
		printf("0x%x\n",registerdata[i]);
	}*/
	closeDevice(&spidevice);
	ADXL345 adxl345;
	initializeADXL345(&adxl345);
	setResolution(&adxl345,NORMAL);
	setRange(&adxl345,PLUSMINUS_4_G);
	displayPitchAndRoll(&adxl345,100);
	closeADXL345Device(&adxl345);
	/*SPIDevice spidevice;
	int i;
	char* registerdata;
	openDevice(&i2cdevice, BBB_I2C_1,0x53);
	writeRegister(&i2cdevice,0x2D,0x08);
	writeRegister(&i2cdevice,0x31,0x00);
	registerdata=readRegisters(&i2cdevice,0x40,0x00);
	for(i =0;i < 0x40;i++){
		printf("0x%x\n",registerdata[i]);
	}
	debugDumpRegisters(&i2cdevice, 32);
	closeDevice(&i2cdevice);*/
	/*ADXL345 adxl345;
	//openDevice(I2CDevice* i2cdevice, char* bus,unsigned int device)
	initializeADXL345(&adxl345,BBB_I2C_1,0x53);
	setResolution(&adxl345, NORMAL);
	setRange(&adxl345,PLUSMINUS_4_G);
	displayPitchAndRoll(&adxl345,100);
	//closeDevice(&i2cdevice);
	closeADXL345Device(&adxl345);*/
	return 0;

}


