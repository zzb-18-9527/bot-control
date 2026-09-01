# 计划：为 NRF24L01 接收机代码增加双电机 PWM 控制

## 一、摘要

在现有的 STM32F103C8T6 + NRF24L01 接收机代码基础上，增加 PWM 输出功能，将收到的遥控摇杆数据转换为 50Hz PWM 信号，通过电调控制双电机船模。实现差速转向控制（左摇杆控制前进基础速度，右摇杆控制差速转向），并加入安全限幅保护。

---

## 二、当前状态分析

### 2.1 硬件资源配置

| 资源 | 引脚 | 状态 |
|------|------|------|
| OLED (SSD1306 I2C) | PA5(SCL), PA7(SDA) | **已使用** |
| NRF24L01 (SPI2) | PB10(CSN), PB11(IRQ), PB12(CE), PB13(SCK), PB14(MISO), PB15(MOSI) | **已使用** |
| TIM3 (1ms定时中断) | 仅内部使用，无输出引脚 | **已使用** |
| TIM2 CH1/CH2 | PA0, PA1 | **空闲 ✅** |
| 其他 GPIO | PA2, PA3, PB0-PB9 等 | **空闲 ✅** |

### 2.2 时钟配置

- **系统主频 (SYSCLK):** 72MHz (HSE = 8MHz, PLL x9)
- **APB1 时钟 (PCLK1):** 36MHz (APB1 prescaler = 2)
- **APB2 时钟 (PCLK2):** 72MHz
- **TIM3 定时器时钟:** 72MHz (APB1 prescaler ≠ 1，TIMx 时钟 = PCLK1 × 2)
- **TIM3 配置:** PSC = 71 (72分频) → 1MHz, ARR = 999 → 1ms中断

### 2.3 NRF24L01 数据格式（已从遥控器代码确认）

| 索引 | 内容 | 值范围 | 来源引脚 |
|------|------|--------|----------|
| [0]-[7] | 按键/开关状态 | 0 或 1 | 各 GPIO |
| [8] | 右侧电位器 | 0-255 | PA0 |
| [9] | 左侧电位器 | 0-255 | PA1 |
| [10] | **左摇杆 X 轴** | 0-255 | PA2 |
| [11] | **左摇杆 Y 轴** ← 前进 | 0-255, 中值≈131 | PA3 |
| [12] | **右摇杆 X 轴** ← 转向 | 0-255, 中值≈131 | PA4 |
| [13] | 右摇杆 Y 轴 | 0-255 | PA5 |

**已确认的摇杆特性：**
- 左 Y (data[11])：中值 **131**，向前推到最前 → **13**（数值减小），向后拉到最前 → **253**（数值增大）
- 右 X (data[12])：中值 **131**，向左推到最左 → **13**，向右推到最右 → **253**
- 发射机在模拟值 ≤110 或 ≥140 时才发送数据（内建死区约 ±20）
- **值域修正：模拟值范围为 13~253，非 0~255**

### 2.4 当前主循环逻辑

1. 轮询接收 NRF24L01 数据包
2. 接收到数据后在 OLED 上显示所有 14 字节的值
3. 对 `data[10]~data[13]` 做简单的阈值判断（≥137 或 ≤118）
4. 500ms 无数据更新则标记指令失效

---

## 三、变更方案

### 3.1 新增文件：PWM 模块

**路径:** `HARDWARE/PWM/pwm.c` 和 `HARDWARE/PWM/pwm.h`

**选型理由:**
- 使用 **TIM2** 产生两路 PWM（CH1 和 CH2）
- 引脚选择 **PA0 (TIM2_CH1)** 和 **PA1 (TIM2_CH2)**——这两个引脚当前完全空闲
- TIM2 与 TIM3 同为 APB1 定时器，时钟特性一致

**PWM 参数计算（基于 72MHz 时钟，APB1 prescaler=2 → TIM2 clock=72MHz）：**

| 参数 | 值 | 说明 |
|------|-----|------|
| PSC (预分频) | 71 | 72MHz / (71+1) = 1MHz |
| ARR (自动重装载) | 19999 | 1MHz / (19999+1) = 50Hz ✅ |
| CCR 最小值 | **1000** | 对应 1ms 脉宽（0 油门） |
| CCR 最大值 | **2000** | 对应 2ms 脉宽（满油门） |
| 安全限幅范围 | 1000 ~ 2000 | 防止超出电调接受范围 |

**函数接口：**
```c
void PWM_Init(void);
void PWM_SetMotor1(uint16_t ccr);  // 设置电机1 PWM, 自动限幅 1000~2000
void PWM_SetMotor2(uint16_t ccr);  // 设置电机2 PWM, 自动限幅 1000~2000
```

### 3.2 新增文件：电机控制模块

**路径:** `HARDWARE/MOTOR/motor.c` 和 `HARDWARE/MOTOR/motor.h`

**功能：**
1. 读取摇杆原始值，进行中值死区判断
2. 将摇杆值映射到 PWM 占空比范围
3. 执行差速转向算法
4. 安全限幅保护

**差速转向算法：**

**Step 1 — 基础速度（左摇杆 Y 轴，data[11]）**
```
前进方向：数值从 131（中值）向下到 13（最大前进）
映射：baseRaw = constrain(data[11], 13, 131)
     baseSpeed = map(baseRaw, 13, 131, 2000, 1000)
结果：data[11]=13  → baseSpeed=2000（满油）
     data[11]=131 → baseSpeed=1000（零油）
死区：data[11] ≥ 121 时 baseSpeed=1000（零油）
```

**Step 2 — 差速量（右摇杆 X 轴，data[12]）**
```
中值 131：
  data[12] < 131 → 向左转 → 增加右电机速度
  data[12] > 131 → 向右转 → 增加左电机速度

向左转：turnRaw = constrain(131 - data[12], 0, 118)  // 131 - 13 = 118
        turnCCR = map(turnRaw, 0, 118, 0, 500)       // 最大差速 500 CCR

向右转：turnRaw = constrain(data[12] - 131, 0, 122)  // 253 - 131 = 122
        turnCCR = map(turnRaw, 0, 122, 0, 500)       // 最大差速 500 CCR
```

**Step 3 — 混合输出**
```
左电机 CCR = baseSpeed + (向右转 ? turnCCR : 0)
右电机 CCR = baseSpeed + (向左转 ? turnCCR : 0)

最终约束：constrain(ccr, 1000, 2000)
```

**信号超时安全保护：**
- 利用现有 `F_2G4_Command_Valid` 标志
- 当 `F_2G4_Command_Valid == 0`（500ms 无更新）时，两路 PWM 均置为 1000（零油）

**函数接口：**
```c
void Motor_Control_Init(void);
void Motor_Update(uint8_t* rx_data, uint8_t command_valid);
```

### 3.3 修改文件：main.c

**变更内容：**
1. 添加头文件引用：`#include "pwm.h"` 和 `#include "motor.h"`
2. 在 `main()` 初始化部分增加：
   - `PWM_Init()` — 在 TIM3 初始化之后
   - `Motor_Control_Init()` — 在 PWM 初始化之后
3. 在主循环 `while(1)` 的数据接收成功分支中：
   - 保留 OLED 显示逻辑（调试用）
   - 替换原有的散乱 if-else 阈值判断，调用 `Motor_Update(RF2G4_Receive_Data, F_2G4_Command_Valid)`
4. 在 `F_2G4_Command_Valid == 0` 分支中增加电机安全归零处理

### 3.4 修改文件：timer.c

**变更内容：**
- 无需修改（TIM3 的 500ms 超时机制已经在正常工作）

---

## 四、已知假设

| # | 假设 | 说明 |
|---|------|------|
| 1 | 向前推左摇杆数值变小 | ✅ **已确认**（中值131，向前→12） |
| 2 | 电机仅单向正转 | ✅ **已确认**用户需求 |
| 3 | 电调为常规航模电调 | 50Hz PWM，1-2ms 脉宽，不带刹车/反转 |
| 4 | TIM2 时钟 = 72MHz | 与 TIM3 相同配置（APB1 prescaler ≠ 1） |
| 5 | 最大差速 500 CCR | 相当于 0.5ms 脉宽增量，可调 |

---

## 五、验证步骤

1. **编译检查：** 在 Keil uVision 中编译项目，确保无语法/链接错误
2. **静态检查：** 确认 PWM 引脚 PA0/PA1 无冲突
3. **上电验证：**
   - 接收机开机，OLED 显示正常
   - 用示波器/逻辑分析仪测量 PA0 和 PA1：频率 50Hz ±1Hz，脉宽 1ms~2ms
4. **功能验证：**
   - 左摇杆推到最前 → 两路 PWM 脉宽约 2ms（满油门）
   - 左摇杆中位 → 两路 PWM 脉宽约 1ms（零油门）
   - 左摇杆推前 + 右摇杆向左 → 右电机 PWM > 左电机 PWM
   - 左摇杆推前 + 右摇杆向右 → 左电机 PWM > 右电机 PWM
   - 关闭遥控器 → 500ms 后两路 PWM 归零 (1ms)

---

## 六、实施清单

- [ ] 创建 `HARDWARE/PWM/pwm.h`
- [ ] 创建 `HARDWARE/PWM/pwm.c`
- [ ] 创建 `HARDWARE/MOTOR/motor.h`
- [ ] 创建 `HARDWARE/MOTOR/motor.c`
- [ ] 修改 `USER/main.c`
- [ ] 编译验证
