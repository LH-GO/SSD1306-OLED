#include <STC12C5A60S2.H>
#include "oled.h"

#define u8 unsigned char
#define u16 unsigned int

delay_ms(u16 ms)
{
	u16 i,j;
	for(i=0;i<ms;i++)
		for(j=846;j>0;j--);
}

void main()
{
	delay_ms(50);
	OLED_Init();
	OLED_Clear();
	OLED_transport(0, 0, 11);//无刷电机控制系统
	OLED_transport(16, 0, 12);
	OLED_transport(32, 0, 13);
	OLED_transport(48, 0, 14);
	OLED_transport(64, 0, 15);
	OLED_transport(80, 0, 16);
	OLED_transport(96, 0, 17);
	OLED_transport(112, 0, 18);
	
	OLED_transport(0, 4, 0);//模式：等待控制
	OLED_transport(16, 4, 1);
	OLED_transport(32, 4, 2);
	OLED_transport(50, 4, 19);
	OLED_transport(66, 4, 20);
	OLED_transport(82, 4, 15);
	OLED_transport(98, 4, 16);
	OLED_Showpicture(1);
	while(1)
	{
		
	}
}