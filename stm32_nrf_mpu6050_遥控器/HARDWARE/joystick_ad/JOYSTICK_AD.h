#ifndef __ad_h
#define __ad_h
#include "stm32f10x.h" 
extern uint16_t AD_Value[6];
#define leftX GPIO_Pin_2
#define leftY GPIO_Pin_3
#define rightX GPIO_Pin_4
#define rightY GPIO_Pin_5
#define dianwei_left GPIO_Pin_0
#define dianwei_right GPIO_Pin_1
void AD_Init(void);


#endif
