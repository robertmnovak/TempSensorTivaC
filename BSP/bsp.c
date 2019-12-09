/* Board Support Package */
#include "bsp.h"
__attribute__((naked)) void assert_failed (char const *file, int line){
    NVIC_SystemReset(); /* reset the system */
}

void SysTick_Init(void){
    SysTick->LOAD = SYS_CLOCK_HZ/2U - 1U;
    SysTick->VAL = 0U;
    SysTick->CTRL = (1U<<2) | (1U<<1) | 1U;
}

void SysTick_Handler(void){
    if(systemCtr == 6){
        systemCtr = 0;
    } else {
        systemCtr++;
    }
}
