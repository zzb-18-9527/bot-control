#include "JOYSTICK_AD.h"
#include "multi_menu.h"   
#include "led.h"
#include "stdio.h"
#include "delay.h"
#include "key.h"

#define frame_line1 u8g2_DrawRBox(&u8g2,0,1,127,16,3)     //以实色画第一行选择框	
#define frame_line2 u8g2_DrawRBox(&u8g2,0,17,127,16,3)    //以实色画第二行选择框	
#define frame_line3 u8g2_DrawRBox(&u8g2,0,33,127,16,3)    //以实色画第三行选择框	
#define frame_line4 u8g2_DrawRBox(&u8g2,0,48,127,16,3)    //以实色画第四行选择框	

#define str_line1(x) u8g2_DrawStr(&u8g2,14,14,x)    //在第一行写字
#define str_line2(x) u8g2_DrawStr(&u8g2,14,30,x)    //在第二行写字
#define str_line3(x) u8g2_DrawStr(&u8g2,14,46,x)    //在第三行写字
#define str_line4(x) u8g2_DrawStr(&u8g2,14,61,x)    //在第四行写字

#define transparent_mode(x) u8g2_SetFontMode(&u8g2,x)     //是否开启透明字体模式，0为默认值不开启透明字体，1开启
#define colour_mode(x) u8g2_SetDrawColor(&u8g2,x)       //设置画笔颜色，0为背景透明色，1为实色

u8g2_t u8g2;

key_table table[30]=
{
	//第0层
	{0,0,0,1,(*fun_0)},
	
    //第1层
	{1,4,2, 5,(*fun_a1)},
	{2,1,3, 9,(*fun_b1)},
	{3,2,4,13,(*fun_c1)},		
	{4,3,1, 0,(*fun_d1)},
	
    //第2层
	{5,8,6,17,(*fun_a21)},					
	{6,5,7,18,(*fun_a22)},
	{7,6,8,19,(*fun_a23)},						                	
	{8,7,5,1,(*fun_a24)},
	
	{ 9,12,10,20,(*fun_b21)},					
	{10, 9,11,21,(*fun_b22)},
	{11,10,12,22,(*fun_b23)},						                	
	{12,11, 9, 2,(*fun_b24)},
	
	{13,16,14,23,(*fun_c21)},					
	{14,13,15,24,(*fun_c22)},				                	
	{15,14,16,25,(*fun_c23)},				                	
	{16,15,13, 3,(*fun_c24)},
	
    //第3层
	{17,17,17,5,(*fun_a31)},			                	
	{18,18,18,6,(*fun_a32)},		                	
	{19,19,19,7,(*fun_a33)},
	
	{20,20,20, 9,(*fun_b31)},				                	
	{21,21,21,10,(*fun_b32)},			                	
	{22,22,22,11,(*fun_b33)},
	
	{23,23,23,13,(*fun_c31)},			                	
	{24,24,24,14,(*fun_c32)},			                	
	{25,25,25,15,(*fun_c33)},								
};

void button_show(){
	
	
}
void joystick_show(){
	
	
}
void potenti_show(u8g2_t *u8g2){
	  u16 advalue1;
	  u16 advalue2;
	  u16 bar_percent1;
	  u16 bar_percent2;
	  char buff[6];
	
	  advalue1 = AD_Value[0]/16;//右电位器值			
		advalue2 = AD_Value[1]/16;//左电位器值
	
	  bar_percent1=(u16)(80.0/256.0*advalue1);	
		bar_percent2=(u16)(80.0/256.0*advalue2);
	
	  sprintf(buff,"%d",advalue1);	  	
		u8g2_DrawStr(u8g2,5,17,buff);
	
		u8g2_DrawRFrame(u8g2,36,5,83,15,3);//圆角矩形
		u8g2_DrawRBox(u8g2,38,7,bar_percent1,11,2);//圆角填充框矩形框
	
	  sprintf(buff,"%d",advalue2);	  	
		u8g2_DrawStr(u8g2,5,49,buff);	
	
		u8g2_DrawRFrame(u8g2,36,36,83,15,3);//圆角矩形
		u8g2_DrawRBox(u8g2,38,38,bar_percent2,11,2);//圆角填充框矩形框
}

/*********第1层***********/
		

void fun_a1()
{	
//u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);	
	u8g2_SetFont(&u8g2, u8g2_font_tenthinnerguys_t_all);
u8g2_SetFontPosBaseline(&u8g2);

transparent_mode(1);
colour_mode(1);
frame_line1;
colour_mode(0);
str_line1("Hardware");		
colour_mode(1);
str_line2("Setting");
str_line3("Device Info");
str_line4("return");

}

void fun_b1()   
{	

transparent_mode(1);
colour_mode(1);
frame_line2;
colour_mode(0);	
str_line2("Setting");	
colour_mode(1);
str_line1("Hardware");
str_line3("Device Info");
str_line4("return");

}

void fun_c1()     
{	

transparent_mode(1);
colour_mode(1);
frame_line3;
colour_mode(0);
str_line3("Device Info");	
colour_mode(1);
str_line1("Hardware");
str_line2("Setting");	
str_line4("return");

}

void fun_d1()     
{	
	
transparent_mode(1);
colour_mode(1);
frame_line4;
colour_mode(0);
str_line4("return");	
colour_mode(1);
str_line1("Hardware");
str_line2("Setting");		
str_line3("Device Info");


}

/*********第2层***********/
void fun_a21()    
{	

transparent_mode(1);
colour_mode(1);
frame_line1;
colour_mode(0);
str_line1("Button");		
colour_mode(1);
str_line2("Joystick");
str_line3("Potentiometer");
str_line4("return");	

}

void fun_a22()      
{	

transparent_mode(1);
colour_mode(1);
frame_line2;
colour_mode(0);
str_line2("Joystick");	
colour_mode(1);
str_line1("Button");
str_line3("Potentiometer");
str_line4("return");		

}

void fun_a23()    
{	

transparent_mode(1);
colour_mode(1);
frame_line3;
colour_mode(0);
str_line3("Potentiometer");	
colour_mode(1);
str_line1("Button");	
str_line2("Joystick");
str_line4("return");		
	
}

void fun_a24()    
{	

transparent_mode(1);
colour_mode(1);
frame_line4;
colour_mode(0);
str_line4("return");		
colour_mode(1);
str_line1("Button");	
str_line2("Joystick");
str_line3("Potentiometer");	

}

void fun_b21()    
{	

transparent_mode(1);
colour_mode(1);
frame_line1;
colour_mode(0);
str_line1("1");		
colour_mode(1);
str_line2("2");
str_line3("3");
str_line4("return");		

}

void fun_b22()    
{	

transparent_mode(1);
colour_mode(1);
frame_line2;
colour_mode(0);
str_line2("2");		
colour_mode(1);
str_line1("1");
str_line3("3");
str_line4("return");		
	
}

void fun_b23()    
{	

transparent_mode(1);
colour_mode(1);
frame_line3;
colour_mode(0);
str_line3("3");		
colour_mode(1);
str_line1("1");
str_line2("2");
str_line4("return");		

}

void fun_b24()    
{	

transparent_mode(1);
colour_mode(1);
frame_line4;
colour_mode(0);
str_line4("return");		
colour_mode(1);
str_line1("1");
str_line2("2");
str_line3("3");		

}

void fun_c21()    
{	

transparent_mode(1);
colour_mode(1);
frame_line1;
colour_mode(0);
str_line1("WiFi Info");		
colour_mode(1);
str_line2("CPU_TEMP");
str_line3("TEMP&HUMI");
str_line4("return");		

}

void fun_c22()    
{	

transparent_mode(1);
colour_mode(1);
frame_line2;
colour_mode(0);
str_line2("CPU_TEMP");	
colour_mode(1);
str_line1("WiFi Info");
str_line3("TEMP&HUMI");
str_line4("return");	

}

void fun_c23()    
{	

transparent_mode(1);
colour_mode(1);
frame_line3;
colour_mode(0);
str_line3("TEMP&HUMI");	
colour_mode(1);
str_line1("WiFi Info");
str_line2("CPU_TEMP");	
str_line4("return");		

}

void fun_c24()    
{	

transparent_mode(1);
colour_mode(1);
frame_line4;
colour_mode(0);
str_line4("return");	
colour_mode(1);
str_line1("WiFi Info");	
str_line2("CPU_TEMP");
str_line3("TEMP&HUMI");			



}

/*********第3层***********/
void fun_a31()    
{	
button_show();
	
}

void fun_a32()    
{	
joystick_show();
	
}

void fun_a33()    
{	
potenti_show(&u8g2);

}

void fun_b31()    
{	
	u8g2_DrawStr(&u8g2,0,16,"*** Music ***");
	u8g2_DrawStr(&u8g2,0,32,"WindyHill");
	u8g2_DrawStr(&u8g2,0,48,"");
	u8g2_DrawStr(&u8g2,0,64,"Enter to Return");

}

void fun_b32()    
{	
	u8g2_DrawStr(&u8g2,0,16,"*** Music ***");
	u8g2_DrawStr(&u8g2,0,32,"New Boy");
	u8g2_DrawStr(&u8g2,0,48,"");
	u8g2_DrawStr(&u8g2,0,64,"Enter to Return");	
	
}

void fun_b33()    
{	
	u8g2_DrawStr(&u8g2,0,16,"*** Music ***");
	u8g2_DrawStr(&u8g2,0,32,"Kill The Rain");
	u8g2_DrawStr(&u8g2,0,48,"");
	u8g2_DrawStr(&u8g2,0,64,"Enter to Return");	
	
}

void fun_c31()    
{	
	u8g2_DrawStr(&u8g2,0,16,"WiFI info");
	u8g2_DrawStr(&u8g2,0,32,"ssid:xxx");
	u8g2_DrawStr(&u8g2,0,48,"passwd:xxx");
	u8g2_DrawStr(&u8g2,0,64,"Enter to Return");

}

void fun_c32()    
{	
	
	u8g2_DrawStr(&u8g2,0,64,"Enter to Return");	

}

void fun_c33()    
{	
	u8g2_DrawStr(&u8g2,0,64,"Enter to Return");

}


/*********第0层***********/
void fun_0(){
	u8g2_SetFont(&u8g2,u8g2_font_koleeko_tu);
		u8g2_DrawStr(&u8g2,25,16,"WELCOME TO");
	u8g2_DrawStr(&u8g2,33,32,"NRF MENU");
	u8g2_DrawStr(&u8g2,1,48,"ENTER TO CONTINUE");
}


