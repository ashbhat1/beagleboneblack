#include "ADXL345.h"
#include "SPIDevice.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>


//From Table 19. of the ADXL345 Data sheet
#define DEVID          0x00   //Device ID
#define THRESH_TAP     0x1D   //Tap Threshold
#define OFSX           0x1E   //X-axis Offset
#define OFSY           0x1F   //Y-axis Offset
#define OFSZ           0x20   //Z-axis Offset
#define DUR            0x21   //Tap duration
#define LATENT         0x22   //Tap latency
#define WINDOW         0x23   //Tap window
#define THRESH_ACT     0x24   //Activity threshold
#define THRESH_INACT   0x25   //Threshold inactivity
#define TIME_INACT     0x26   //Inactivity time
#define ACT_INACT_CTL  0x27   //Axis enable control for activity and inactivity detection
#define THRESH_FF      0x28   //Free-fall threshold
#define TIME_FF        0x29   //Free-fall time
#define TAP_AXES       0x2A   //Axis control for single tap/double tap
#define ACT_TAP_STATUS 0x2B   //Source of single tap/double tap
#define BW_RATE        0x2C   //Data rate and power mode control
#define POWER_CTL      0x2D   //Power-saving features control
#define INT_ENABLE     0x2E   //Interrupt enable control
#define INT_MAP        0x2F   //Interrupt mapping control
#define INT_SOURCE     0x30   //Source of interrupts
#define DATA_FORMAT    0x31   //Data format control
#define DATAX0         0x32   //X-axis Data 0
#define DATAX1         0x33   //X-axis Data 1
#define DATAY0         0x34   //Y-axis Data 0
#define DATAY1         0x35   //Y-axis Data 1
#define DATAZ0         0x36   //Z-axis Data 0
#define DATAZ1         0x37   //Z-axis Data 1
#define FIFO_CTL       0x38   //FIFO control
#define FIFO_STATUS    0x39   //FIFO status


void initializeADXL345(ADXL345* adxl345){
	SPIDevice spidevice;
	adxl345->spidevice=spidevice;
	openDevice(&adxl345->spidevice);
	//adxl345->spidevice=spidevice;
	adxl345->accelerationX=0;
	adxl345->accelerationY=0;
	adxl345->accelerationZ=0;
	adxl345->pitch=0;
	adxl345->roll =0;
	adxl345->registers=NULL;
	adxl345->range=PLUSMINUS_16_G;
	adxl345->resolution=HIGH;
}

short combineRegisters(char msb,char lsb){
	return ((short)msb<<8)|(short)lsb;
}

void calculatePitchAndRoll(ADXL345* adxl345){
	double gravity_range;
	switch(adxl345->range){
		case PLUSMINUS_16_G: gravity_range=32.0; break;
		case PLUSMINUS_8_G : gravity_range=16.0; break;
		case PLUSMINUS_4_G : gravity_range=8.0 ; break;
		default            : gravity_range=4.0 ; break;
	}
	double resolution=1024.0;
	if(adxl345->resolution==HIGH){resolution=8192.0;}
	float factor = gravity_range/resolution;
	float accXg=adxl345->accelerationX;
	float accYg=adxl345->accelerationY;
	float accZg=adxl345->accelerationZ;
	float accXSquared=accXg*accXg;
	float accYSquared=accYg*accYg;
	float accZSquared=accZg*accZg;
	adxl345->pitch=180 * atan(accXg/sqrt(accYSquared + accZSquared))/M_PI;
	adxl345->roll =180 * atan(accYg/sqrt(accXSquared + accZSquared))/M_PI;
	//printf("pitch: %0.4f\n",adxl345->pitch);
	//printf("roll:  %0.4f\n",adxl345->roll);
}

int updateRegisters(ADXL345* adxl345){
	char data_format = 0x00;
	data_format = data_format|((adxl345->resolution)<<3);
	data_format = data_format|adxl345->range;
	return spi_writeRegister(&adxl345->spidevice,DATA_FORMAT,data_format);//this needs to be part of
}

void setRange(ADXL345* adxl345, enum RANGE range){
	adxl345->range=range;
	updateRegisters(adxl345);
}

void setResolution(ADXL345* adxl345, enum RESOLUTION resolution){
	adxl345->resolution = resolution;
	updateRegisters(adxl345);
}

int readSensorState(ADXL345* adxl345){
	//adxl345->registers=malloc(sizeof(char) * BUFFER_SIZE);
	adxl345->registers = readRegisters(&adxl345->spidevice,BUFFER_SIZE,0x00);
	int idx =0;
	if(*(adxl345->registers)!=0xe5){
		perror("ADXL345 - Failure Condition - Sensor ID cannot be verified\n");
		return -1;
	}
	//printf("ADXL345 range: %d\n",adxl345->range);
	//char* data;
	//data=readRegisters(&adxl345->spidevice,6,DATAX0);//why does this change adxl345->register
	int i= 0;
	short temp;
	//temp=combineRegisters(*(data+(i+1)),*(data+i));
	//printf("readRegister1: 0x%x\n",(int)*(data));
	//printf("ReadRegister1.1: 0x%x\n",(int)*(data+1));
	printf("readRegister2: 0x%x\n",(int)*(adxl345->registers));
	adxl345->accelerationX = combineRegisters(*(adxl345->registers+DATAX1), *(adxl345->registers+DATAX0));
	adxl345->accelerationY = combineRegisters(*(adxl345->registers+DATAY1), *(adxl345->registers+DATAY0));
	adxl345->accelerationZ = combineRegisters(*(adxl345->registers+DATAZ1), *(adxl345->registers+DATAZ0));
	printf("accelerationX: %d\n",adxl345->accelerationX);
	printf("accelerationY %d\n",adxl345->accelerationY);
	printf("accelerationZ %d\n",adxl345->accelerationZ);
	adxl345->resolution = ((*(adxl345->registers+DATA_FORMAT))&0x08);
	adxl345->range = ((*(adxl345->registers+DATA_FORMAT))&0x03);
	//printf("resolution %d\n",adxl345->resolution);
	//printf("range %d\n",adxl345->range);
	calculatePitchAndRoll(adxl345);
	return 0;
}

void displayPitchAndRoll(ADXL345* adxl345,int iterations){
	int count =0;
	while (count<iterations){
		//printf("Pitch: %0.4f Roll: %0.4f\n",adxl345->pitch,adxl345->roll);
		usleep(100000);
		readSensorState(adxl345);
		count++;
	}
}

void closeADXL345Device(ADXL345* adxl345){
	closeDevice(&adxl345->spidevice);
}


