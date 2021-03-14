#ifndef __OLED_H_
#define __OLED_H_

#include <STC12C5A60S2.H>
#include <intrins.h>

#define u8 unsigned char
#define u16 unsigned int

#define IIC_SCL P01 
#define IIC_SDA P02
#define OLED_CMD 1
#define OLED_DATA 0


void IIC_Delay1us();
void IIC_Start();
void IIC_Stop();
void IIC_Wait_Ack();
void IIC_Write_Byte(u8 IIC_Byte);

void IIC_Write_OLcmd(u8 IIC_Cmd);
void IIC_Write_OLdata(u8 IIC_data);
void OLED_Write_Byte(u8 dat,u8 cmd);
void OLED_Set_Pos(u8 x, u8 y);

void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_On(void);
void OLED_Clear(void);
void OLED_ShowCHinese(u8 x,u8 y,u8 num);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Size);
void OLED_transport(u8 x, u8 y, u8 num);
void OLED_Showpicture(u8 Hanz);
void OLED_Init();


#endif