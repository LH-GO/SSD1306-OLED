#include "oled.h"
#include "oledcases.h"

void IIC_Delay1us()		//@12.000MHz
{
	_nop_();
	_nop_();
}

void delay1us(u8 x)
{
	u8 i;
	for(i=0;i<x;i++)
	{
		_nop_();
	}
}

void IIC_Start()
{
	IIC_SDA=1;
	IIC_SCL=1;
	IIC_SDA=0;
	IIC_Delay1us();
	IIC_SCL=0;
	IIC_Delay1us();
}

void IIC_Stop()
{
	IIC_SDA=0;
	IIC_SCL=1;
	IIC_SDA=1;
	IIC_Delay1us();
}

void IIC_Wait_Ack()
{
	IIC_SCL=1;
	IIC_SCL=0;
}

//int IIC_Wait_Ack()
//{
//	IIC_SDA = 1;
//	delay1us(5);
//	IIC_SCL = 1;
//	delay1us(5);
//	CY=IIC_SDA;
//	IIC_SCL = 0;
//	delay1us(5);
//	return CY;
//}

void IIC_Write_Byte(u8 IIC_Byte)
{
	u8 i;
	u8 da;
	IIC_SCL=0;
	for(i=0;i<8;i++)
	{
		da=IIC_Byte&0x80;
		if(da==0x80)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		IIC_Byte=IIC_Byte<<1;
		delay1us(5);
		IIC_SCL=1;
		delay1us(5);
		IIC_SCL=0;
		delay1us(5);
	}
}

void IIC_Write_OLcmd(u8 IIC_Cmd)
{
	IIC_Start();
	IIC_Write_Byte(0x78);
	IIC_Wait_Ack();
	IIC_Write_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Write_Byte(IIC_Cmd);
	IIC_Wait_Ack();
	IIC_Stop();
}

void IIC_Write_OLdata(u8 IIC_data)
{
	IIC_Start();
	IIC_Write_Byte(0x78);
	IIC_Wait_Ack();
	IIC_Write_Byte(0x40);
	IIC_Wait_Ack();
	IIC_Write_Byte(IIC_data);
	IIC_Wait_Ack();
	IIC_Stop();
}

void OLED_Write_Byte(u8 dat,u8 cmd)
{
	if(cmd)
	{
		IIC_Write_OLcmd(dat);
	}
	else
	{
		IIC_Write_OLdata(dat);
	}
}

void OLED_Set_Pos(u8 x, u8 y) //显示位置的坐标
{ 	
	OLED_Write_Byte(0xb0+y,OLED_CMD);
	OLED_Write_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_Write_Byte((x&0x0f),OLED_CMD); 
} 

void OLED_Display_On(void)
{
	OLED_Write_Byte(0x8D,OLED_CMD);
	OLED_Write_Byte(0x14,OLED_CMD);
	OLED_Write_Byte(0xAF,OLED_CMD);
}

void OLED_Display_Off(void)
{
	OLED_Write_Byte(0x8D,OLED_CMD);
	OLED_Write_Byte(0x10,OLED_CMD);
	OLED_Write_Byte(0xAE,OLED_CMD);
}

void OLED_On(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_Write_Byte(0xb0+i,OLED_CMD);
		OLED_Write_Byte(0x00,OLED_CMD);
		OLED_Write_Byte(0x10,OLED_CMD);
		for(n=0;n<128;n++)OLED_Write_Byte(0xff,OLED_DATA);
	}
}

void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_Write_Byte(0xb0+i,OLED_CMD);
		OLED_Write_Byte(0x00,OLED_CMD);
		OLED_Write_Byte(0x10,OLED_CMD);
		for(n=0;n<128;n++)OLED_Write_Byte(0x00,OLED_DATA);
	}
}

/*******显示ASCII表里的字符*******/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Size)
{
	u8 i,c;
	c=chr-' ';
	if(x>127){x=0;y=y+1;}
	if(Size==16)//8*16
	{
		OLED_Set_Pos(x, y);
		for(i=0;i<8;i++)
		OLED_Write_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x, y+1);
		for(i=0;i<8;i++)
		OLED_Write_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else//6*8
	{
		OLED_Set_Pos(x, y);
		for(i=0;i<6;i++)
		OLED_Write_Byte(F8X16[c*16+i],OLED_DATA);
	}
}


/*********显示汉字*********/
void OLED_ShowCHinese(u8 x,u8 y,u8 num)
{
	u8 i;
	OLED_Set_Pos(x, y);
	for(i=0;i<16;i++)
	{
		OLED_Write_Byte(Hzk[2*num][i],OLED_DATA);
	}
	OLED_Set_Pos(x, y+1);
	for(i=0;i<16;i++)
	{
		OLED_Write_Byte(Hzk[2*num+1][i],OLED_DATA);
	}
}

/*********最后把字符数组传递到图片数组中*********/
void OLED_transport(u8 x, u8 y, u8 num)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		Draw_trans[y][x+i]=Hzk[2*num][i];
	}
	for(i=0;i<16;i++)
	{
		Draw_trans[y+1][x+i]=Hzk[2*num+1][i];
	}
}

/*********Hanz=1字符显示，Hanz=0图片显示*********/
void OLED_Showpicture(u8 Hanz)
{
	u8 i,n;
	if(Hanz)
	{
		for(i=0;i<8;i++)
		{
			OLED_Write_Byte(0xb0+i,OLED_CMD);
			OLED_Write_Byte(0x00,OLED_CMD);
			OLED_Write_Byte(0x10,OLED_CMD);
			for(n=0;n<128;n++)OLED_Write_Byte(Draw_trans[i][n],OLED_DATA);
		}
		for(i=0;i<8;i++)
			for(n=0;n<128;n++) Draw_trans[i][n]=0;
		
	}
	else
	{
		for(i=0;i<8;i++)
		{
			OLED_Write_Byte(0xb0+i,OLED_CMD);
			OLED_Write_Byte(0x00,OLED_CMD);
			OLED_Write_Byte(0x10,OLED_CMD);
			for(n=0;n<128;n++)OLED_Write_Byte(Draw[i][n],OLED_DATA);
		}
		for(i=0;i<8;i++)
			for(n=0;n<128;n++) Draw_trans[i][n]=0;
	}
	
}

/*********OLED初始化*********/
void OLED_Init()
{
	OLED_Write_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_Write_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_Write_Byte(0x80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_Write_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_Write_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_Write_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_Write_Byte(0X00,OLED_CMD); //默认为0

	OLED_Write_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_Write_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_Write_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_Write_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_Write_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_Write_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_Write_Byte(0xC8,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_Write_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_Write_Byte(0x12,OLED_CMD); //[5:4]配置
		 
	OLED_Write_Byte(0x81,OLED_CMD); //对比度设置
	OLED_Write_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_Write_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_Write_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_Write_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_Write_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_Write_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_Write_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_Write_Byte(0xAF,OLED_CMD); //开启显示	 
	OLED_Clear();

}


