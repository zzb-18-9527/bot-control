#include "led.h"


// 初始化PB4、PB5为推挽输出
//-----------------------------------------------------------------------------------------
void LED_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;					// 定义GPIO_InitTypeDef型结构体

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);		// PB4的JTRST失能
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 使能GPIOB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;	// 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		// IO口速度为10MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// 
	
	GPIO_SetBits(GPIOA,GPIO_Pin_11);		//电源显示
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);//发射显示
}
//-----------------------------------------------------------------------------------------
