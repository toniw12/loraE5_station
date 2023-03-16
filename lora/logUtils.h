/*
 * logUtils.h
 *
 *  Created on: Dec 7, 2022
 *      Author: antoine
 */

#ifndef LOGUTILS_H_
#define LOGUTILS_H_

//#define logPrintf()



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
int	logPrintf (const char *format, ...);
void logBuffer(const char * func,uint16_t addr,uint8_t *buffer, uint16_t size);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* LOGUTILS_H_ */
