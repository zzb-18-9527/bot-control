 //���ߣ�
//NFR:  MOSI-PB15
//      MISO-PB14
//		  CE  -PB12
//			IRQ -PB11
//      SCK -PB13
//      CSN -PB10
//OLED
//      SCL-A5
//      SDA-A7
#include "u8g2.h"
#include "stm32f10x.h"
#include "bit_band.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "spi.h"
#include "rf2G4.h"
#include "u8g2_init.h"
#include "pwm.h"
#include "motor.h"
#include <stdio.h>

// ȫ�ֱ���
//---------------------------------------------------------
u16 C_TIM2_IT_Update = 0 ;		// TIM3�Ķ�ʱ�ƴ�

u16 C_2G4_Command = 0 ;			// 2.4Gָ���ʱ

u8  F_2G4_Command_Valid = 0 ;	// 2.4G������Ч��־λ
//---------------------------------------------------------


int main(void)
{	

	// ע��������ʹ���ж�ʱ��NVIC��������Ӧ����λ�ڳ�����ʼ�������������ú�����Ҫ�ٸ���NVIC����
	//------------------------------------------------------------------------------------------
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //NVIC����2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	delay_init();				// ��ʱ��ʼ����ע��������ʱ����֮ǰ�������ȵ���delay_Init()��SysTick��ʼ����
	
	//LED_Init() ;				// ��ʼ��LEDӲ���ӿ�
	
	TIM3_Time_Init();			// ��ʱ��3��ʼ��
	
	RF2G4_Init();				// 2.4G�����Ƶ��ʼ��
	
	PWM_Init();					// PWM��ʼ��, 50Hz, PA0/PA1
	
	Motor_Control_Init();		// �������ģ���ʼ��
	
	IIC_Init();	    //u8g2DrawTest iic��ʼ��
	u8g2_t u8g2;
	u8g2Init(&u8g2);	
	u8g2_SetFont(&u8g2,u8g2_font_ncenB08_tf);
	
	u8g2_ClearBuffer(&u8g2); 
	u8g2_DrawStr(&u8g2,15,15,"2.4G receive");
	u8g2_SendBuffer(&u8g2); 

	//---------------------------------------------
	if(RF2G4_Check())	// �ж�SI24R1�Ƿ�����
	{ u8g2_DrawStr(&u8g2,15, 30, "NRF ERROR"); 
		u8g2_SendBuffer(&u8g2); 
	}
	else
	{ u8g2_DrawStr(&u8g2,15, 30, "NRF OK"); }
	u8g2_SendBuffer(&u8g2); 
	//------------------------------------------------
	RF2G4_RX_Mode();			// ��SI24R1����Ϊ����ģʽ
	//-----------------------------------------------------		

	
	while(1)
	{
		if(RF2G4_Rx_Packet(RF2G4_Receive_Data,14) == RX_SUCCESS)	// �ж��Ƿ���յ�����	
		{		
			//-----------------------------------------------
			F_2G4_Command_Valid = 1 ;	// 2.4Gָ����Чλ��1
			
			C_2G4_Command = 0 ;			// ���¶�2.4Gָ���ʱ
			//-----------------------------------------------		�������ڽ�������oled����ʾ���е�ҡ�ˡ���ťֵ
			char buf[6];
			u8g2_ClearBuffer(&u8g2); 
			
			sprintf(buf,"%d",RF2G4_Receive_Data[0]);
			u8g2_DrawStr(&u8g2,40,45,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[1]);
			u8g2_DrawStr(&u8g2,40,60,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[2]);
			u8g2_DrawStr(&u8g2,60,15,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[3]);
			u8g2_DrawStr(&u8g2,60,30,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[4]);
			u8g2_DrawStr(&u8g2,60,45,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[5]);
			u8g2_DrawStr(&u8g2,60,60,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[6]);
			u8g2_DrawStr(&u8g2,70,15,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[7]);
			u8g2_DrawStr(&u8g2,70,30,buf);	
			
			sprintf(buf,"%d",RF2G4_Receive_Data[8]);	    
	    u8g2_DrawStr(&u8g2,15,15,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[9]);
			u8g2_DrawStr(&u8g2,15,30,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[10]);
			u8g2_DrawStr(&u8g2,15,45,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[11]);
			u8g2_DrawStr(&u8g2,15,60,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[12]);
			u8g2_DrawStr(&u8g2,40,15,buf);
			
			sprintf(buf,"%d",RF2G4_Receive_Data[13]);
			u8g2_DrawStr(&u8g2,40,30,buf);
								
	    u8g2_SendBuffer(&u8g2);
			
		// ���ݽ��յ���ҡ�����ݸ��µ����PWM���
			Motor_Update(RF2G4_Receive_Data, F_2G4_Command_Valid);
		}
		
	else{//û�յ���Ϣ������ʾno receive
	u8g2_ClearBuffer(&u8g2); 
	u8g2_DrawStr(&u8g2,15,45,"no receive");
	u8g2_SendBuffer(&u8g2);
	}		
		
		//---------------------------------------------------
		if( F_2G4_Command_Valid == 0)// ÿ��0.5s�ж��ж������Ƿ�ʧЧ,�����������0.5s��û�и��£����ж�Ϊ��Ч�����������б���ָ���ֹС������ʧ��
		{
			// �źų�ʱ: Motor_Update�ڲ����command_valid=0ʱ�Ѿ�������ȫ����
		}

		
	}
}


#define SEND_BUFFER_DISPLAY_MS(u8g2, ms)\
  do {\
    u8g2_SendBuffer(u8g2); \
    delay_ms(ms);\
  }while(0);
  
