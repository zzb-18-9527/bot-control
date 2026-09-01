#include "key.h"

// 按键初始化
//---------------------
void KEY_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;				// 定义GPIO_InitTypeDef型结构体
	
	// 使能GPIOA、GPIOC端口时钟
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	
	#if WKUP_EN	
	//---------------------------------------------------------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// 使能GPIOA时钟
	
	// 按键：WKUP  ---- PA0 端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				// WKUP <==> PA_in(0)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 			// 下拉输入
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 输入模式不需要设端口速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// 根据设定参数初始化PA0
	//---------------------------------------------------------------------------------
	#endif
	
	
	#if KEY1_EN	
	//---------------------------------------------------------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	// 使能GPIOC时钟
	
	// 按键：KEY_1  ---- PC13 端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				// 按键：KEY_1  ---- PC13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			// 上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);					// 根据设定参数初始化PC13
	//---------------------------------------------------------------------------------
	#endif
}
