#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

// 摇杆参数常量
#define JOYSTICK_CENTER     131     // 摇杆中值
#define JOYSTICK_MIN        13      // 摇杆最小值
#define JOYSTICK_MAX        253     // 摇杆最大值
#define JOYSTICK_DEADZONE   10      // 中值死区范围

// PWM 参数常量
#define PWM_CCR_MIN         1000    // 0 油门 (1ms)
#define PWM_CCR_MAX         2000    // 满油门 (2ms)
#define TURN_CCR_MAX        500     // 最大差速增量

void Motor_Control_Init(void);
void Motor_Update(uint8_t *rx_data, uint8_t command_valid);

#endif
