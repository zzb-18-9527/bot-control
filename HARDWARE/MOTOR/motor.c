#include "motor.h"
#include "pwm.h"

//--------------------------------------------------------------------------------------------------------------
// 限幅函数
//--------------------------------------------------------------------------------------------------------------
static uint16_t constrain_u16(uint16_t val, uint16_t min, uint16_t max)
{
    if (val < min) val = min;
    if (val > max) val = max;
    return val;
}

//--------------------------------------------------------------------------------------------------------------
// 初始化电机控制模块
//--------------------------------------------------------------------------------------------------------------
void Motor_Control_Init(void)
{
    // PWM 已在外部初始化, 此处仅确保初始输出为 0 油门
    PWM_SetMotor1(PWM_CCR_MIN);
    PWM_SetMotor2(PWM_CCR_MIN);
}

//--------------------------------------------------------------------------------------------------------------
// 主映射函数: 将左 Y 轴向前区域映射为 PWM 占空比
// data[11] = 左 Y 轴
//   131 (中值) → 1000 (零油)
//   13  (最大前推) → 2000 (满油)
//--------------------------------------------------------------------------------------------------------------
static uint16_t map_forward_speed(uint8_t left_y)
{
    uint16_t baseRaw;

    // 死区处理: 中值附近不动作
    if (left_y >= JOYSTICK_CENTER - JOYSTICK_DEADZONE)
        return PWM_CCR_MIN;

    // 限幅到有效范围 [JOYSTICK_MIN, JOYSTICK_CENTER]
    baseRaw = constrain_u16(left_y, JOYSTICK_MIN, JOYSTICK_CENTER);

    // 映射: 13→2000, 131→1000 (线性递减关系)
    // speed = 1000 + (131 - value) * (2000-1000) / (131-13)
    //       = 1000 + (131 - value) * 1000 / 118
    return (uint16_t)(PWM_CCR_MIN + (uint32_t)(JOYSTICK_CENTER - baseRaw) * (PWM_CCR_MAX - PWM_CCR_MIN) / (JOYSTICK_CENTER - JOYSTICK_MIN));
}

//--------------------------------------------------------------------------------------------------------------
// 右 X 轴差速映射
// data[12] = 右 X 轴
//   131 (中值) → 差速 0
//   13  (最左) → 向左转, 差速增量 500 (右电机加速)
//   253 (最右) → 向右转, 差速增量 500 (左电机加速)
//--------------------------------------------------------------------------------------------------------------
static void map_turn(uint8_t right_x, uint16_t *turn_left, uint16_t *turn_right)
{
    *turn_left = 0;
    *turn_right = 0;

    // 死区处理
    if (right_x > JOYSTICK_CENTER - JOYSTICK_DEADZONE &&
        right_x < JOYSTICK_CENTER + JOYSTICK_DEADZONE)
        return;

    if (right_x < JOYSTICK_CENTER)
    {
        // 向左转: 增加右电机速度
        uint16_t turnRaw = constrain_u16(JOYSTICK_CENTER - right_x, 0, JOYSTICK_CENTER - JOYSTICK_MIN);
        *turn_left = 0;
        *turn_right = (uint16_t)((uint32_t)turnRaw * TURN_CCR_MAX / (JOYSTICK_CENTER - JOYSTICK_MIN));
    }
    else
    {
        // 向右转: 增加左电机速度
        uint16_t turnRaw = constrain_u16(right_x - JOYSTICK_CENTER, 0, JOYSTICK_MAX - JOYSTICK_CENTER);
        *turn_left = (uint16_t)((uint32_t)turnRaw * TURN_CCR_MAX / (JOYSTICK_MAX - JOYSTICK_CENTER));
        *turn_right = 0;
    }
}

//--------------------------------------------------------------------------------------------------------------
// 更新电机输出
// rx_data: NRF24L01 接收到的 14 字节数据
// command_valid: 指令有效标志 (0=超时失效, 1=有效)
//--------------------------------------------------------------------------------------------------------------
void Motor_Update(uint8_t *rx_data, uint8_t command_valid)
{
    uint16_t baseSpeed, turnLeft, turnRight;
    uint16_t motor1_ccr, motor2_ccr;

    if (command_valid == 0)
    {
        // 信号超时: 双电机归零 (安全保护)
        PWM_SetMotor1(PWM_CCR_MIN);
        PWM_SetMotor2(PWM_CCR_MIN);
        return;
    }

    // Step 1: 基础速度 (左 Y 轴)
    baseSpeed = map_forward_speed(rx_data[11]);

    // Step 2: 差速量 (右 X 轴)
    map_turn(rx_data[12], &turnLeft, &turnRight);

    // Step 3: 混合输出
    // 左电机 = base + (向右转时增加的差速)
    motor1_ccr = constrain_u16(baseSpeed + turnRight, PWM_CCR_MIN, PWM_CCR_MAX);
    // 右电机 = base + (向左转时增加的差速)
    motor2_ccr = constrain_u16(baseSpeed + turnLeft, PWM_CCR_MIN, PWM_CCR_MAX);

    PWM_SetMotor1(motor1_ccr);
    PWM_SetMotor2(motor2_ccr);
}
