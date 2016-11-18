/*
 * SPIDevice.h
 *
 *  Created on: Nov 13, 2016
 *      Author: genia
 */


#ifndef SPIDEVICE_H_
#define SPIDEVICE_H_

#include<string.h>
#include<stdint.h>

#define SPI_PATH "/dev/spidev1.0"

enum SPIMODE{
	MODE0 = 0,
	MODE1 = 1,
	MODE2 = 2,
	MODE3 = 3
};

typedef struct{
	char* device;
	enum SPIMODE mode;
	int file;
	uint8_t bits;
	uint16_t delay;
	uint32_t speed;
}SPIDevice;

enum SPIMODE mode;
uint8_t bits;
uint32_t speed;
uint16_t delay;
int spi_transfer(SPIDevice* spidevice,char send[],char recieve[], int length);
char readRegister(SPIDevice* spidevice,unsigned int registerAddress);
char* readRegisters(SPIDevice* spidevice,unsigned int number,unsigned int fromAddress);
int closeDevice(SPIDevice* spidevice);
int openDevice(SPIDevice* spidevice);
int spi_write(SPIDevice* spidevice,char* value,int length);
int spi_writeRegister(SPIDevice* spidevice,unsigned int registerAddress,char value);
void debugDumpRegisters(SPIDevice* spidevice,unsigned int number);
int setSpeed(SPIDevice* spidevice,uint32_t speed);
int setMode(SPIDevice* spidevice,enum SPIMODE mode);
int setBitsPerWord(SPIDevice* spidevice,uint8_t bits);

#endif /* SPIDEVICE_H_ */

