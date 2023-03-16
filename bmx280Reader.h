/*
 * ADCfilteredReader.h
 *
 *  Created on: Feb 2, 2023
 *      Author: antoine
 */

#ifndef BMX280READER_H_
#define BMX280READER_H_

#include "Arduino.h"
#include "BMx280TwoWire.h"

enum {
    BMx280_TEMPERATURE,
    BMx280_PRESSURE_MSB,
    BMx280_PRESSURE_LSB,
    BMx280_HUMIDITY
};

class BMx280Reader {
public:
    BMx280Reader();
    BMx280Reader(TwoWire *wire);
    virtual ~BMx280Reader();
    void begin();
    void measure();

    uint16_t registers[4];
    float pressure,temperature,humidity;
protected:
    TwoWire wire_;
    BMx280TwoWire bmx280;
};

#endif /* BMX280READER_H_ */
