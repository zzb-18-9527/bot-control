#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

// PWM参数常量
#define PWM_TIM_CLK        72000000    // TIM2 输入时钟 72MHz
#define PWM_PSC            71          // 预分频 72 分频 → 1MHz
#define PWM_ARR            19999       // 自动重装载值 → 50Hz (1MHz / 20000)
#define PWM_CCR_MIN        1000        // 最小脉宽 1ms (0 油门)
#define PWM_CCR_MAX        2000        // 最大脉宽 2ms (满油门)

void PWM_Init(void);
void PWM_SetMotor1(uint16_t ccr);      // 设置电机1 PWM, 自动限幅
void PWM_SetMotor2(uint16_t ccr);      // 设置电机2 PWM, 自动限幅

#endif
