#include "led.h"


// 初始化PA1、PA2
//-----------------------------------------------------------------------------------------
void LED_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;					// 定义GPIO_InitTypeDef型结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 使能GPIOA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_0|GPIO_Pin_9;	// LED1--PA1、LED2--PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// IO口速度为10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// 
	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);				// PA1、PA2输出高
	
}
//-----------------------------------------------------------------------------------------
