/*
 * interruptDebug.h
 *
 *  Created on: Dec 9, 2022
 *      Author: antoine
 */

#ifndef INTERRUPTDEBUG_H_
#define INTERRUPTDEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif
void debug_printf(const char *fmt, ...);
void triggerUsageFault();

#ifdef __cplusplus
}
#endif
#endif /* INTERRUPTDEBUG_H_ */
