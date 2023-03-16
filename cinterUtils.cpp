/*
 * cinterUtils.cpp
 *
 *  Created on: Feb 8, 2023
 *      Author: antoine
 */
#include "cinterUtils.h"


//extern HardwareSerial Serial(PB7, PB6);
extern ModbusRTUClientClass ModbusRTUClient;
extern SdFat sd;

constexpr unsigned int hashstr2int(const char *str, int h) {
    return !str[h] ? 5381 : (hashstr2int(str, h + 1) * 33) ^ str[h];
}


void printFileContent(char *filename) {
    File myFile = sd.open(filename);
    if (myFile) {
        Serial.println("File content:");
        while (myFile.available()) {
            Serial.write(myFile.read());
        }
    } else {
        Serial.println("Error opening file");
    }
    myFile.close();
}


int readModbus(int id, int type, int address, int nb, unsigned short *data) {
    if (!ModbusRTUClient.requestFrom(id, type, address, nb)) {
        Serial.print("failed to read registers! ");
        Serial.println(ModbusRTUClient.lastError());
        return -1;
    } else {
        for (int i = 0; i < nb; i++) {
            data[i] = (unsigned short) ModbusRTUClient.read();
        }
        return 0;
    }
}

int readSensors(JsonDocument &outputData,JsonArray &configStruct,uint16_t rawdata[]) {
    int rawdataptr=0;
    for (JsonVariant value : configStruct) {
        const char *sensorName = value["name"] | "sensor";
        int id = value["id"] | 1;
        int type = INPUT_REGISTERS;
        switch (hashstr2int((char*) (value["type"] | "INPUT_REGISTERS"))) {
        case hashstr2int("DISCRETE_INPUTS"):
            type = DISCRETE_INPUTS;
            break;
        case hashstr2int("INPUT_REGISTERS"):
            type = INPUT_REGISTERS;
            break;
        case hashstr2int("HOLDING_REGISTERS"):
            type = HOLDING_REGISTERS;
            break;
        default:
            Serial.println("cannot define type");
        }
        int address = value["address"] | 0;
        int nb = value["nb"] | 1;
        JsonArray data = outputData.createNestedArray((String)sensorName);
        unsigned short mbdata[16];
        if (!readModbus(id, type, address, nb, mbdata)) {
            //Serial.print(sensorName);
            //Serial.print(": ");
            for (int i = 0; i < nb; i++) {
                //Serial.print(mbdata[i]);
                //Serial.print(",");
                rawdata[rawdataptr++]=mbdata[i];
                data.add(mbdata[i]);
            }
            //Serial.println();
        } else {
            for (int i = 0; i < nb; i++) {
                rawdata[rawdataptr++]=0x7FFF;
            }
        }
    }
    return rawdataptr;
}

void logFile(String fileName, String data) {
    File dataFile = sd.open(fileName, O_APPEND | O_WRITE | O_CREAT);
    // if the file is available, write to it:
    if (dataFile) {
        dataFile.println(data);
        dataFile.close();
        // print to the debug port too:
    }
}

void setSensorPower(uint8_t power) {
    Wire.beginTransmission(0x20);  // Transmit to device number 44 (0x2C)
    Wire.write(power);
    Wire.endTransmission();
}



