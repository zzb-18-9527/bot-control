#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"


extern u8 key_scan[8];


// 按键输入宏定义
//------------------------------------------------------------------------
#define 	menu_mode				PBin(8)	//转化到菜单按钮
#define 	menu_up					PCin(15)	//菜单上调按钮，也是上按钮
#define 	menu_down				PCin(13)	//菜单下调按钮，也是下按钮
#define   menu_enter      PBin(1) //菜单确认按钮，也是右按钮
#define 	key_left				PCin(14)	//左按钮
#define 	rocker_left			  PBin(7)	//左摇杆按钮
#define 	rocker_right			PAin(6)	//右遥感按钮
#define 	lever_left				PAin(7)	// 左拨杆
#define 	lever_right				PAin(9)	// 右拨杆



// 按键初始化
//----------------------------
void KEY_Init_JX(void);

// 按键扫描函数
//----------------------------
void RF2G4_KEY_Scan_JX(void);


#endif	
