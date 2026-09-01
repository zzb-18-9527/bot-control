#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"

// 定义为1表示使能该按键，定义为0表示失能该按键
//------------------------------------------
#define 	WKUP_EN		1		// 使能
#define 	KEY1_EN		1		// 使能
//------------------------------------------

// 按键宏定义
//-----------------------------------------------------
#define 	WKUP_in		PA_in(0)	// WKUP <==> PA_in(0)
#define 	KEY1_in		PC_in(13)	// KEY1 <==> PC_in(13)


void KEY_Init(void) ;


#endif	/* __KEY_H */
