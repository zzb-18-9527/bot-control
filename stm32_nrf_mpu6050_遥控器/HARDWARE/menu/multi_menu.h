#ifndef __MULTI_MENU1_H
#define __MULTI_MENU1_H	 
#include "u8g2.h"
#define uchar unsigned char
	typedef struct
{
	uchar current;
	uchar up;//向上翻索引号
	uchar down;//向下翻索引号
	uchar enter;//确认索引号
	void (*current_operation)();
} key_table;

extern key_table table[30];

extern u8g2_t u8g2;

extern void fun_a1(void); 
extern void fun_b1(void);  
extern void fun_c1(void);  
extern void fun_d1(void);  

extern void fun_a21(void); 
extern void fun_a22(void);  
extern void fun_a23(void);  
extern void fun_a24(void);  

extern void fun_b21(void); 
extern void fun_b22(void);  
extern void fun_b23(void);  
extern void fun_b24(void); 

extern void fun_c21(void); 
extern void fun_c22(void);  
extern void fun_c23(void);  
extern void fun_c24(void);  

extern void fun_a31(void); 
extern void fun_a32(void);  
extern void fun_a33(void);  

extern void fun_b31(void); 
extern void fun_b32(void);  
extern void fun_b33(void); 

extern void fun_c31(void); 
extern void fun_c32(void);  
extern void fun_c33(void); 
  
extern void fun_0(void);   


#endif
