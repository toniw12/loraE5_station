/*
 * interruptDebug.c
 *
 *  Created on: Dec 9, 2022
 *      Author: antoine
 */

#include <interruptDebug.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include "Arduino.h"
#include "uart.h"
#include "core_cm4.h"
#include "stm32_def.h"
#include "stm32yyxx_ll_usart.h"

void usart_transmit_data(const void* data, uint16_t len) {
    const uint8_t* b = data;
    while (len--) {
        while (!LL_USART_IsActiveFlag_TXE(USART1));
        LL_USART_TransmitData8(USART1, *b++);
    }
}

void vprint(const char *fmt, va_list argp)
{
    char string[200];
    if(0 < vsprintf(string,fmt,argp)) // build string
    {
    	usart_transmit_data((uint8_t*)string, strlen(string));
    }
}

void debug_printf(const char *fmt, ...) // custom printf() function
{
    va_list argp;
    va_start(argp, fmt);
    vprint(fmt, argp);
    va_end(argp);
}

void triggerUsageFault(){
	/* Fill a meaningless value */
	*(uint32_t*)0x20010000 = 0xFFFFFFFF;

	/* Set PC with LSB being 1 to indicate Thumb State */
	void (*pFunc)(void) = (void*)0x20010001;

	/* call function */
	pFunc();
}

__attribute__ ((naked)) void BusFault_Handler(void) {
    // get current Stack Pointer
    __asm volatile("MRS R0, MSP");
    __asm volatile("B Default_Handler_main");
}


__attribute__ ((naked)) void UsageFault_Handler(void) {
    // get current Stack Pointer
    __asm volatile("MRS R0, MSP");
    __asm volatile("B Default_Handler_main");
}

void Default_Handler_main(uint32_t* pMSP) {

  //USART_TypeDef * usartInst=USART2_BASE;//0x40004400;
 // while(!(usartInst->ISR & USART_ISR_TC)){
//	  USART2_IRQHandler();
  //}
  //USART2_IRQHandler();
  debug_printf("Exception: Usage Fault\n");
  DumpExceptionRegister(pMSP);

  uint32_t* pUFSR = (uint32_t*)0xE000ED2A;
  debug_printf("UFSR = 0x%lx\n", *pUFSR & 0xFFFF);

  NVIC_SystemReset();
  while(1);
}

void DumpExceptionRegister(uint32_t* pMSP)
{
	debug_printf(" MSP = %p\n", pMSP);
	debug_printf("  R0 = 0x%8lx\n", pMSP[0]);  // May have argument of function
	debug_printf("  R1 = 0x%8lx\n", pMSP[1]);  // May have argument of function
	debug_printf("  R2 = 0x%8lx\n", pMSP[2]);  // May have argument of function
	debug_printf("  R3 = 0x%8lx\n", pMSP[3]);  // May have argument of function
	debug_printf(" R12 = 0x%8lx\n", pMSP[4]);  // IP holds an intermediate value of a calculation
	debug_printf("  LR = 0x%8lx\n", pMSP[5]);  // Address of the next instruction before the exception
	debug_printf("  PC = 0x%8lx\n", pMSP[6]);  // CPU was executing the instruction at PC
	debug_printf("xPSR = 0x%8lx\n", pMSP[7]);  // Status of system before execution at PC completes

	debug_printf("  PC = 0x%8lx\n", pMSP[6]);  // CPU was executing the instruction at PC
	debug_printf("xPSR = 0x%8lx\n", pMSP[7]);

	debug_printf("ICSR = 0x%8lx\n", SCB->ICSR);  // CPU was executing the instruction at PC
	debug_printf("SHCSR= 0x%8lx\n", SCB->SHCSR);

}
