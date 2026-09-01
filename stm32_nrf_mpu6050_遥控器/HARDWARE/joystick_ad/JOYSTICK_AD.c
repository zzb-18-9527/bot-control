#include "stm32f10x.h"    
#include "JOYSTICK_AD.h"
#include "delay.h"
// Device header
//stm32c8t6的PA0-PA7、PB0、PB1为可用的ADC输入引脚
uint16_t AD_Value[6];

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//时钟使能
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = leftX | leftY | rightX | rightY|dianwei_left|dianwei_right;//6个ADC输入口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);//2个电位器
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_55Cycles5);//4个摇杆	
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//内部软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//使用扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 6;//通道数
	ADC_Init(ADC1, &ADC_InitStructure);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//外设站点地址为ADC的DR寄存器，即ADC结果值存放的寄存器
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//半字，即16个bit
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设站点地址不自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址自增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设为源头，即外设传给内存
	DMA_InitStructure.DMA_BufferSize = 6;//传输计数器值为4
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//传输计数器自动重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//硬件出发，触发源为ADC1每次转换完成
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//ADC1通道在硬件上接到DMA1，所以必须为DMA1
	
	DMA_Cmd(DMA1_Channel1, ENABLE);//开启DMA，由于没有触发源，所以该函数后DMA不会开始转运，需要等待ADC触发
	ADC_DMACmd(ADC1, ENABLE);//开启ADC1到DMA的触发通道
	ADC_Cmd(ADC1, ENABLE);//开启 adc
	
	//如需要，还可加入其他外设，比如使用定时器定时触发adc，adc触发dma，不消耗cpu资源

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//给ADC1一个软件触发，随后ADC和dma一直连续工作，将结果运至AD_Value[6];
}

//u16 Get_Adc_Average(u8 times)//平均值滤波函数
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=adc_getvalue();
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 	 

