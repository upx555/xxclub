#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"                  // Device header

void PWM_INIT(void);
void PWM_SPEEDA1(u8 Compare);
void PWM_SPEEDB0(u8 Compare);
void PWM_INIT1(void);


#endif
