/*
 * ADCfilteredReader.h
 *
 *  Created on: Feb 2, 2023
 *      Author: antoine
 */

#ifndef ADCFILTEREDREADER_H_
#define ADCFILTEREDREADER_H_

#include "Arduino.h"

class ADCfilteredReader {
public:
    ADCfilteredReader(uint32_t pin,float factor=1);
    virtual ~ADCfilteredReader();
    void begin();
    uint16_t measure();
protected:
    uint32_t pin_;
    float factor_;
    int32_t valueFilter=-1;
};

#endif /* ADCFILTEREDREADER_H_ */
