//接线：
//NFR:  MOSI-PB15
//      MISO-PB14
//		  CE  -PB12
//			IRQ -PB11
//      SCK -PB13
//      CSN -PB10
//遥感:
//      左手x轴-PA2
//      左手Y轴-PA3
//      左手按钮-PB7
//      右手x轴-PA4
//      右手Y轴-PA5
//      右手按钮-PA6    
//电位器:
//      定位器1-PA1
//      电位器2-PA0  
//OLED:
//      SDA-PB5
//      SCL-PB9
//自锁按钮:
//      按钮1-PB8   菜单切换
//      按钮2-PA7   lever_right
//      按钮3-PA9   lever_left
//点动按钮:
//      按钮1-PC15     menu_up   上
//      按钮2-PC13  menu_down  下
//      按钮3-PB1  menu_enter  右
//      按钮4-PC14  左
//led
//      led1-PA11
//      led2-PA12
// MPU6050
//      SCL-PB6    
//      SDA-PA8
//      INT-PA10
//
//RF2G4_Send_Data[0] 上点动按钮
//RF2G4_Send_Data[1] 下点动按钮
//RF2G4_Send_Data[2] 确认点动按钮
//RF2G4_Send_Data[3] 左点动按钮
//RF2G4_Send_Data[4] 左摇杆按钮
//RF2G4_Send_Data[5] 右摇杆按钮
//RF2G4_Send_Data[6] 左自锁按钮
//RF2G4_Send_Data[7] 右自锁按钮
//RF2G4_Send_Data[8] 右电位器值
//RF2G4_Send_Data[9] 左电位器值
//RF2G4_Send_Data[10] 左手摇杆上下
//RF2G4_Send_Data[11] 左手摇杆左右
//RF2G4_Send_Data[12] 右手摇杆上下
//RF2G4_Send_Data[13] 右手摇杆左手

//目前版本的代码未加入mpu6050部分

#include "stm32f10x.h" 
#include "bit_band.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "multi_menu.h"//oled菜单库
#include "u8g2.h"
#include <stdio.h>
#include "JOYSTICK_AD.h"//摇杆库
#include "u8g2_init.h"
#include "spi.h"
#include "rf2G4.h"
#include "key.h"

void startup_UI(u8g2_t *u8g2);//开机画面函数

void (*current_operation_index)();
uchar func_index = 0; //0为初始显示欢迎界面，1直接显示菜单栏
//float Pitch,Roll,Yaw;
int main(void)
{		
	u8 L_CNT = 0 ;				// 循环计数
	u8 J_2G4_Data_Send = 0;		// 标志位：判断是否需发射数据
//	
//	// 注：程序中使用中断时，NVIC分组设置应尽量位于程序起始处，并且在设置后尽量不要再更改NVIC分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //NVIC分组2：2位抢占优先级，2位响应优先级
	
	delay_init();				
	
	LED_Init() ;				//led初始化
	
	KEY_Init_JX() ;				// 初始化按键硬件接口
	  
  AD_Init();            //ADC初始化
	
	RF2G4_Init();				// nrf2401 2.4G无线射频初始化
 
	IIC_Init();	        // u8g2 iic初始化

	//u8g2_t u8g2;
	
	u8g2Init(&u8g2);	 //u8g2初始化
	
  startup_UI(&u8g2); //oled开机画面
	
	RF2G4_TX_Mode();	// 将NRF07设置为发射模式
	

	while(1)
		{		
			if(menu_mode==1){//如果菜单切换自锁按钮不按下，则为遥控模式
			u8g2_SetFont(&u8g2,u8g2_font_ncenB08_tf);
   	  RF2G4_KEY_Scan_JX();//检测按键是否按下
									
			for(L_CNT=0;L_CNT<8;L_CNT++)
			{
				RF2G4_Send_Data[L_CNT] = key_scan[L_CNT];//将按键值传入RF2G4_Send_Data数组中
			}
			
			RF2G4_Send_Data[8] = AD_Value[0]/16;//右电位器值。除16为了将16位的平均值转为8位，以能够存入8位的数组
			
			RF2G4_Send_Data[9] = AD_Value[1]/16;//左电位器值
			
			RF2G4_Send_Data[10] = AD_Value[2]/16;//左手摇杆上下
			
			RF2G4_Send_Data[11] = AD_Value[3]/16;//左手摇杆左右
			
			RF2G4_Send_Data[12] = AD_Value[4]/16;//右手摇杆上下
			
			RF2G4_Send_Data[13] = AD_Value[5]/16;//右手摇杆左右//以上六句将摇杆和电位器值传入RF2G4_Send_Data数组中
	//-------------------------------------------------------------//以下部分将按键、摇杆（本质也是电位器）、电位器值显示在屏幕上		
			char buf[6];
			
			u8g2_ClearBuffer(&u8g2);	
			
		  sprintf(buf,"%d",RF2G4_Send_Data[0]);
			u8g2_DrawStr(&u8g2,64,48,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[1]);
			u8g2_DrawStr(&u8g2,64,62,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[2]);
			u8g2_DrawStr(&u8g2,71,55,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[3]);
			u8g2_DrawStr(&u8g2,57,55,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[4]);
			u8g2_DrawStr(&u8g2,37,30,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[5]);
			u8g2_DrawStr(&u8g2,91,30,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[6]);
			u8g2_DrawStr(&u8g2,76,8,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[7]);
			u8g2_DrawStr(&u8g2,50,8,buf);	
			
			sprintf(buf,"%d",RF2G4_Send_Data[8]);     
	    u8g2_DrawStr(&u8g2,106,8,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[9]);
			u8g2_DrawStr(&u8g2,8,8,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[10]);
			u8g2_DrawStr(&u8g2,30,20,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[11]);
			u8g2_DrawStr(&u8g2,30,40,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[12]);
			u8g2_DrawStr(&u8g2,84,20,buf);
			
			sprintf(buf,"%d",RF2G4_Send_Data[13]);
			u8g2_DrawStr(&u8g2,84,40,buf);
							
	    u8g2_SendBuffer(&u8g2); 
	//----------------------------------------------------------------------------------							
			// 判断按键/摇杆的位置（无按键/中间档位，不发射，降低功耗）
			for(L_CNT=0; L_CNT<8; L_CNT++)
			{
				J_2G4_Data_Send += RF2G4_Send_Data[L_CNT];//所有按钮值的和
			}	
			
	   if( J_2G4_Data_Send>0 || RF2G4_Send_Data[8]<=110 || RF2G4_Send_Data[8]>=140 || RF2G4_Send_Data[9]<=110 || RF2G4_Send_Data[9]>=140 || RF2G4_Send_Data[10]<=110 || RF2G4_Send_Data[10]>=140|| RF2G4_Send_Data[11]<=110 || RF2G4_Send_Data[11]>=140|| RF2G4_Send_Data[12]<=110|| RF2G4_Send_Data[12]>=140|| RF2G4_Send_Data[13]<=110|| RF2G4_Send_Data[13]>=140)
			{		
			RF2G4_Tx_Packet(RF2G4_Send_Data,14);// 发射指令,长度只能是14，任何其他数字都会发射失败，目前不知道原因				
			if (RF2G4_Tx_Packet(RF2G4_Send_Data,14)==0){
			GPIO_SetBits(GPIOA,GPIO_Pin_12); 				//发射成功，则点亮led
			} 	
			else{
      GPIO_ResetBits(GPIOA,GPIO_Pin_12);     //发射失败，灯灭
			}				
		}
   else{
		 GPIO_ResetBits(GPIOA,GPIO_Pin_12);    //未发射，灯灭
	 }		 
		//delay_ms(10);
	}
		
else{ //如果菜单切换自锁按钮按下，则为菜单模式，此时不能遥控
			if((menu_up==0)||(menu_down==0)||(menu_enter==0))
		{
			delay_ms(10);//消抖
			if(menu_up==0)
			{
				func_index = table[func_index].up;    //向上翻
				while(!menu_up);//松手检测
			}
			if(menu_down==0)
			{
				func_index = table[func_index].down;    //向下翻
				while(!menu_down);
			}
			if(menu_enter==0)
			{
				func_index = table[func_index].enter;    //确认
				while(!menu_enter);
			}
		}	
	current_operation_index = table[func_index].current_operation;			
			u8g2_ClearBuffer(&u8g2); 
			(*current_operation_index)();//执行当前操作函数
		  u8g2_SendBuffer(&u8g2);
	}	

}	
}		
	
void startup_UI(u8g2_t *u8g2)//开机画面
{
	for(int i=10;i<=85;i=i+5)
	{
		u8g2_ClearBuffer(u8g2); 
			//--------------------------------------------------------//画进度条
		char buff[10];
		sprintf(buff,"%d%%",(int)(i/85.0*100));		
		u8g2_SetFont(u8g2,u8g2_font_michaelmouse_tu);
		u8g2_DrawStr(u8g2,95,38,buff);//当前进度显示
		
		u8g2_DrawRBox(u8g2,2,28,i,11,3);//圆角填充框矩形框
		u8g2_DrawRFrame(u8g2,0,26,89,15,4);//圆角矩形
		//------------------------------------------------------------
	  u8g2_DrawStr(u8g2,9,15,"LOADING...");
		u8g2_SendBuffer(u8g2);
	}
		if(RF2G4_Check()){//检测nrf是否正常
	u8g2_DrawStr(u8g2,9, 60, "NRF ERROR"); //不正常则oled显示error
	u8g2_SendBuffer(u8g2); 
			delay_ms(5000);
	}
	else{
	 u8g2_DrawStr(u8g2,11, 60, "NRF OK"); //正常则显示ok
   u8g2_SendBuffer(u8g2); 
		delay_ms(2000); 
	}
}


#define SEND_BUFFER_DISPLAY_MS(u8g2, ms)\
  do {\
    u8g2_SendBuffer(u8g2); \
    delay_ms(ms);\
  }while(0);
  
