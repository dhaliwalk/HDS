// Code provided by Dr. Fraser
// Demo application to read analog input voltage 1 on the BeagleBone
// Assumes ADC cape already loaded by uBoot:

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include "utils.h"
#include "lock.h"

#define A2D_FILE_VOLTAGE1  "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V  1.8
#define A2D_MAX_READING    4095

#define VOLTAGE_WET 0.05
#define VOLTAGE_0_2_CM 0.68
#define VOLTAGE_0_4_CM 0.83
#define VOLTAGE_0_5_CM 1.12
#define VOLTAGE_1_CM 1.26
#define VOLTAGE_1_5_CM 1.54
#define VOLTAGE_2_CM 1.62
#define VOLTAGE_2_5_CM 1.66
#define VOLTAGE_3_CM 1.7
#define VOLTAGE_3_5_CM 1.77
#define VOLTAGE_4_CM 1.79

float WaterLevelSensor_getVoltage1Reading()
{
	adc_lock.lock();
	// Open file
	FILE *f = fopen(A2D_FILE_VOLTAGE1, "r");
	if (!f) {
		printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
		printf("       Check /boot/uEnv.txt for correct options.\n");
		exit(-1);
	}

	// Get reading
	int a2dReading = 0;
	int itemsRead = fscanf(f, "%d", &a2dReading);
	if (itemsRead <= 0) {
		printf("ERROR: Unable to read values from voltage input file.\n");
		exit(-1);
	}

	// Close file
	fclose(f);
	if (a2dReading == 0) {
		a2dReading++;
	}
	adc_lock.unlock();
	float voltageReading = a2dReading*(A2D_VOLTAGE_REF_V/A2D_MAX_READING)*2;

	if (voltageReading < VOLTAGE_WET) {
		return 0;
	}
	else if (voltageReading < VOLTAGE_0_2_CM) {
		return 0.1;
	}
	else if (voltageReading < VOLTAGE_0_4_CM) {
		return 0.2;
	}
	else if (voltageReading < VOLTAGE_0_5_CM) {
		return 0.4;
	}
	else if (voltageReading < VOLTAGE_1_CM) {
		return 0.5;
	}
	else if (voltageReading < VOLTAGE_1_5_CM) {
		return 1;
	}
	else if (voltageReading < VOLTAGE_2_CM) {
		return 1.5;
	}
	else if (voltageReading < VOLTAGE_2_5_CM) {
		return 2;
	}
	else if (voltageReading < VOLTAGE_3_CM) {
		return 2.5;
	}
	else if (voltageReading < VOLTAGE_3_5_CM) {
		return 3;
	}
	else if (voltageReading < VOLTAGE_4_CM) {
		return 3.5;
	}
	else {
		return 4.0;
	}
}



