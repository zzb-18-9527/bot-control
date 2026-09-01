#include "pwm.h"

//--------------------------------------------------------------------------------------------------------------
// 初始化 TIM2 产生两路 50Hz PWM
// CH1 → PA0 (电机1)
// CH2 → PA1 (电机2)
//--------------------------------------------------------------------------------------------------------------
void PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 使能 GPIOA 和 TIM2 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 配置 PA0, PA1 为复用推挽输出 (TIM2_CH1, TIM2_CH2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // TIM2 时基配置: 72MHz / 72 = 1MHz, ARR = 19999 → 50Hz
    TIM_TimeBaseStructure.TIM_Period = PWM_ARR;
    TIM_TimeBaseStructure.TIM_Prescaler = PWM_PSC;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // CH1 (PA0) PWM1 模式, 预装载使能
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = PWM_CCR_MIN;   // 初始为 0 油门
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // CH2 (PA1) PWM1 模式, 预装载使能
    TIM_OCInitStructure.TIM_Pulse = PWM_CCR_MIN;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE);  // 使能 TIM2
}

//--------------------------------------------------------------------------------------------------------------
// 设置电机1 (PA0) 的 PWM 占空比, 自动限幅到安全范围
//--------------------------------------------------------------------------------------------------------------
void PWM_SetMotor1(uint16_t ccr)
{
    if (ccr < PWM_CCR_MIN) ccr = PWM_CCR_MIN;
    if (ccr > PWM_CCR_MAX) ccr = PWM_CCR_MAX;
    TIM_SetCompare1(TIM2, ccr);
}

//--------------------------------------------------------------------------------------------------------------
// 设置电机2 (PA1) 的 PWM 占空比, 自动限幅到安全范围
//--------------------------------------------------------------------------------------------------------------
void PWM_SetMotor2(uint16_t ccr)
{
    if (ccr < PWM_CCR_MIN) ccr = PWM_CCR_MIN;
    if (ccr > PWM_CCR_MAX) ccr = PWM_CCR_MAX;
    TIM_SetCompare2(TIM2, ccr);
}
