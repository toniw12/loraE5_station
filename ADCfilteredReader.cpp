/*
 * ADCfilteredReader.cpp
 *
 *  Created on: Feb 2, 2023
 *      Author: antoine
 */

#include <ADCfilteredReader.h>

ADCfilteredReader::ADCfilteredReader(uint32_t pin,float factor) :
    pin_(pin),factor_(factor)
{

}

ADCfilteredReader::~ADCfilteredReader() {
    // TODO Auto-generated destructor stub
}

void ADCfilteredReader::begin(){
    analogReadResolution(12);
}

uint16_t ADCfilteredReader::measure(){
    int newVal=analogRead(pin_);
    if(valueFilter==-1){
        valueFilter=newVal<<4;
    }
    else{
        valueFilter=((valueFilter*15)>>4)+newVal;
    }
    return (uint16_t)valueFilter*factor_;
}

