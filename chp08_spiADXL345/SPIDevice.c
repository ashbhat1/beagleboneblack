/*
 * SPIDevice.c
 *
 *  Created on: Nov 14, 2016
 *      Author: genia
 */

#include"SPIDevice.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<stdint.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


#define HEX(x) printf("0x%x\n",(int)(x)); //!< Macro for filling in leading 0 on HEX outputs

int openDevice(SPIDevice* spidevice){
	int file;
	spidevice->device = SPI_PATH;
	spidevice->bits = 8;
	spidevice->speed = 500000;
	spidevice->delay = 0;
	spidevice->mode  = MODE3;
	if((file=open(spidevice->device,O_RDWR))<0){
		perror("SPI: Can't open the device.");
		return -1;
	}
	spidevice->file=file;
	if(setMode(spidevice,spidevice->mode)==-1) return -1;
	if(setSpeed(spidevice,spidevice->speed)==-1) return -1;
	if(setBitsPerWord(spidevice,spidevice->bits)==-1) return -1;
	return 0;
}

int closeDevice(SPIDevice* spidevice){
	close(spidevice->file);
	printf("File has been closed\n");
}

int spi_transfer(SPIDevice* spidevice,char send[],char recieve[], int length){
	struct spi_ioc_transfer transfer;
	memset(&transfer,0,sizeof(transfer));
	transfer.rx_buf=(unsigned long) recieve;
	transfer.tx_buf=(unsigned long) send;
	transfer.len=length;
	transfer.speed_hz=spidevice->speed;
	transfer.bits_per_word=spidevice->bits;
	transfer.delay_usecs=spidevice->delay;
	int status = ioctl(spidevice->file,SPI_IOC_MESSAGE(1),&transfer);
	if (status<0){
		perror("SPI: SPI_IOC_MESSAGE Failed\n.");
		return -1;
	}
	return status;
}

char readRegister(SPIDevice* spidevice,unsigned int registerAddress){
	char send[2],recieve[2];
	memset(send,0,sizeof(send));
	memset(recieve,0,sizeof(recieve));
	send[0]=(char)(0x80|registerAddress);
	spi_transfer(spidevice,send,recieve,2);
	return recieve[1];
}


char* readRegisters(SPIDevice* spidevice,unsigned int number,unsigned int fromAddress){
	char data[number];
	char send[number+1],recieve[number+1];
	memset(send,0,sizeof(send));
	send[0]=(char)(0x80|0x40|fromAddress);
	spi_transfer(spidevice,send,recieve,number+1);
	memcpy(&data,recieve+1,number);
	return data;
}

int spi_write(SPIDevice* spidevice,char* value,int length){
	char null_return=0x00;
	spi_transfer(spidevice,value,&null_return,length);
	return 0;
}

int spi_writeRegister(SPIDevice* spidevice,unsigned int registerAddress,char value){
	char send[2],recieve[2];
	memset(recieve,0,sizeof(recieve));
	send[0]=(char)registerAddress;
	send[1]=value;
	spi_transfer(spidevice,send,recieve,2);
	return 0;
}


int setSpeed(SPIDevice* spidevice,uint32_t speed){
	spidevice->speed = speed;
	if (ioctl(spidevice->file, SPI_IOC_WR_MAX_SPEED_HZ, &(spidevice->speed))==-1){
		perror("SPI: Can't set max speed HZ");
		return -1;
	}
	if (ioctl(spidevice->file, SPI_IOC_RD_MAX_SPEED_HZ, &(spidevice->speed))==-1){
		perror("SPI: Can't get max speed HZ.");
		return -1;
	}
	return 0;
}

int setMode(SPIDevice* spidevice,enum SPIMODE mode){
	spidevice->mode = mode;
	if (ioctl(spidevice->file, SPI_IOC_WR_MODE, &(spidevice->mode))==-1){
		perror("SPI: Can't set SPI mode.");
		return -1;
	}
	if (ioctl(spidevice->file, SPI_IOC_RD_MODE, &(spidevice->mode))==-1){
		perror("SPI: Can't get SPI mode.");
		return -1;
	}
	return 0;
}

int setBitsPerWord(SPIDevice* spidevice,uint8_t bits){
	spidevice->bits = bits;
	if (ioctl(spidevice->file, SPI_IOC_WR_BITS_PER_WORD, &(spidevice->bits))==-1){
		perror("SPI: Can't set bits per word.");
		return -1;
	}
	if (ioctl(spidevice->file, SPI_IOC_RD_BITS_PER_WORD, &(spidevice->bits))==-1){
		perror("SPI: Can't get bits per word.");
		return -1;
	}
	return 0;
}

void debugDumpRegisters(SPIDevice* spidevice,unsigned int number){
	printf("SPI Mode: %d\n",spidevice->mode);
	printf("Bits per word: %d\n",(int)spidevice->bits);
	printf("Max speed: %d\n",spidevice->speed);
	printf("Dumping Registers for Debug Purposes: \n");
	int i=0;
	char *registers = readRegisters(spidevice,number,0x00);
	for(i=0; i<(int)number; i++){
		HEX(*(registers+i));printf(" ");
		if (i%16==15) printf("\n");
	}
}
