/* C version for ADXL345 which integrates with
SPIDevice*/

#ifndef ADXL345_H_
#define ADXL345_H_
#include "SPIDevice.h"

#define BUFFER_SIZE 0x40

enum RANGE {
	PLUSMINUS_2_G  = 0,
	PLUSMINUS_4_G  = 1,
	PLUSMINUS_8_G  = 2,
	PLUSMINUS_16_G = 3
};

enum RESOLUTION {
	NORMAL  = 0,
	HIGH    = 1
};

typedef struct {
	SPIDevice spidevice;
	char* registers;
	enum RANGE range;
	enum RESOLUTION resolution;
	short accelerationX, accelerationY, accelerationZ;
	double pitch, roll;
} ADXL345;

void initializeADXL345(ADXL345* adxl345);
short combineRegisters(char msb,char lsb);
void calculatePitchAndRoll(ADXL345* adxl345);
int updateRegisters(ADXL345* adxl345);
void setRange(ADXL345* adxl345, enum RANGE range);
void setResolution(ADXL345* adxl345, enum RESOLUTION resolution);
int readSensorState(ADXL345* adxl345);
void displayPitchAndRoll(ADXL345* adxl345,int iterations);
void closeADXL345Device(ADXL345* adxl345);

#endif /*ADXL345_H_*/
