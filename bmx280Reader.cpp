/*
 * ADCfilteredReader.cpp
 *
 *  Created on: Feb 2, 2023
 *      Author: antoine
 */

#include <bmx280Reader.h>

#define I2C_ADDRESS_BMX 0x76

BMx280Reader::BMx280Reader() :
    bmx280(&Wire,I2C_ADDRESS_BMX)
{
    volatile int i=35;

}

BMx280Reader::BMx280Reader(TwoWire *wire) :
    bmx280(wire,I2C_ADDRESS_BMX)
{
    volatile int i=36;

}

BMx280Reader::~BMx280Reader() {
    // TODO Auto-generated destructor stub
}

void BMx280Reader::begin(){
    if (!bmx280.begin())
    {
        Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
        for (int i=0;i<4;i++){
            registers[i]=0x7FFF;
        }
        return;
    }
    //reset sensor to default parameters.
    bmx280.resetToDefaults();

    //by default sensing is disabled and must be enabled by setting a non-zero
    //oversampling setting.
    //set an oversampling setting for pressure and temperature measurements.
    bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
    bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

    //if sensor is a BME280, set an oversampling setting for humidity measurements.
    if (bmx280.isBME280())
        bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);

    bmx280.measure();
}

void BMx280Reader::measure(){
    if (bmx280.hasValue()){
        temperature=bmx280.getTemperature();
        if (temperature==NAN){
            registers[BMx280_TEMPERATURE]=0x7FFF;
        }
        else{
            registers[BMx280_TEMPERATURE]=(uint16_t)(temperature*100);
        }

        pressure=bmx280.getPressure();
        if (pressure==NAN){
            registers[BMx280_PRESSURE_MSB]=0x7FFF;
            registers[BMx280_PRESSURE_LSB]=0xFFFF;
        }
        else{
            registers[BMx280_PRESSURE_MSB]=(uint16_t)(((int32_t)pressure)>>16);
            registers[BMx280_PRESSURE_LSB]=(uint16_t)((int32_t)pressure)&0xffff;
        }
        
        if (bmx280.isBME280())
        {
            humidity=bmx280.getHumidity();
            if (humidity==NAN){
                registers[BMx280_HUMIDITY]=0x7FFF;
            }
            else{
                registers[BMx280_HUMIDITY]=(uint16_t)(humidity*100);
            }
        }
        else{
            registers[BMx280_HUMIDITY]=0x7FFF;
        }
    }
    bmx280.measure();
}

