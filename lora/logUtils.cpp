/*
 * logUtils.cpp
 *
 *  Created on: Dec 7, 2022
 *      Author: antoine
 */

#include "Arduino.h"

#include "logUtils.h"


#include <stdio.h>
#include <stdarg.h>

extern HardwareSerial debug;

extern "C" {


int logPrintf(const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  int retval = debug.print( format); //,ap
  va_end(ap);
  //return retval;
  HAL_Delay(10);
  return 0;
}
void logBuffer(const char * func,uint16_t addr,uint8_t *buffer, uint16_t size){

    debug.print( func);
    /*
    debug.printf( "%s(0x%x,[",func,addr);
	for(int i=0;i<size;i++){
		if(i==size-1){
		    debug.printf( "0x%02x",buffer[i]);
		}
		else{
		    debug.printf( "0x%02x,",buffer[i]);
		}
	}
	debug.printf( "])\n\r");

	*/
    //HAL_Delay(10);
}

}

