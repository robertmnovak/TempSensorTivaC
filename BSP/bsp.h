#ifndef __BSP_H__
#define __BSP_H__
#include "TM4C123GH6PM.h"

void SysTick_Init(void);

#define SYS_CLOCK_HZ 16000000U

uint8_t systemCtr;


#endif //__BSP_H__
