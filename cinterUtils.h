/*
 * cinterHeader.h
 *
 *  Created on: Feb 8, 2023
 *      Author: antoine
 */

#ifndef CINTERUTILS_H_
#define CINTERUTILS_H_


/*
 * cinterUtils.cpp
 *
 *  Created on: Feb 8, 2023
 *      Author: antoine
 */
#include "Arduino.h"

#include <SdFat.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <Wire.h>



constexpr unsigned int hashstr2int(const char *str, int h = 0);
void printFileContent(char *filename);
int readModbus(int id, int type, int address, int nb, unsigned short *data) ;
int readSensors(JsonDocument &outputData,JsonArray &configStruct,uint16_t rawdata[]);
void logFile(String fileName, String data);
void setSensorPower(uint8_t power);
time_t compileTime();

#endif /* CINTERUTILS_H_ */
