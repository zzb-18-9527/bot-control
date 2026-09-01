#include "key.h"
#define cnt 1



// 8个按键的状态：按下为0，松开为1
// 写入数组状态：按下写1，松开写0
//-------------------------------
u8 key_scan[8] = { 0 };
//-------------------------------

// 按键初始化
//---------------------
void KEY_Init_JX(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		// JTAG失能
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 输入模式不需要设端口速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//上下左右四个按钮，其中三个菜单按钮 PC13,14,15,PB1
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//左右摇杆（自锁）按钮
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
	GPIO_Init(GPIOA, &GPIO_InitStructure);			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
	GPIO_Init(GPIOB, &GPIO_InitStructure);//三个自锁 PA7,9,PB8
						
}

// 按键扫描函数
//------------------------------------------
void RF2G4_KEY_Scan_JX(void)// menu_up
{
//	if(menu_up)
//	{ menu_up_Cnt = 0; key_scan[0] = 0;}
//	else
//	{	menu_up_Cnt ++ ;
//		if( menu_up_Cnt>cnt )
//		{ key_scan[0] = 1; }
//	}
//////////////////////////////////////	
	if(menu_up==0){
		delay_ms(10);
		if(menu_up==0){
		 key_scan[0] = 1;
		}
	}
	else key_scan[0] = 0;
			
	////////////////////////////////
		if(menu_down==0){
		delay_ms(10);
		if(menu_down==0){
		 key_scan[1] = 1;
		}
	}
	else key_scan[1] = 0;
	
	/////////////////////////////////
		if(menu_enter==0){
		delay_ms(10);
		if(menu_enter==0){
		 key_scan[2] = 1;
		}
	}
	else key_scan[2] = 0;
//////////////////////////////////////////
		if(key_left==0){
		delay_ms(10);
		if(key_left==0){
		 key_scan[3] = 1;
		}
	}
	else key_scan[3] = 0;

/////////////////////////////////////////
		if(rocker_left==0){
		delay_ms(10);
		if(rocker_left==0){
		 key_scan[4] = 1;
		}
	}
	else key_scan[4] = 0;

	////////////////////////////////////////
		if(rocker_right==0){
		delay_ms(10);
		if(rocker_right==0){
		 key_scan[5] = 1;
		}
	}
	else key_scan[5] = 0;


		if(lever_left==0){
		delay_ms(10);
		if(lever_left==0){
		 key_scan[6] = 1;
		}
	}
	else key_scan[6] = 0;


		if(lever_right==0){
		delay_ms(10);
		if(lever_right==0){
		 key_scan[7] = 1;
		}
	}
	else key_scan[7] = 0;
	
}


