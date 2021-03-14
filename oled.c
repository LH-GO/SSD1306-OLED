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

void OLED_Set_Pos(u8 x, u8 y) //��ʾλ�õ�����
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

/*******��ʾASCII������ַ�*******/
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


/*********��ʾ����*********/
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

/*********�����ַ����鴫�ݵ�ͼƬ������*********/
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

/*********Hanz=1�ַ���ʾ��Hanz=0ͼƬ��ʾ*********/
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

/*********OLED��ʼ��*********/
void OLED_Init()
{
	OLED_Write_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_Write_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_Write_Byte(0x80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_Write_Byte(0xA8,OLED_CMD); //��������·��
	OLED_Write_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_Write_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_Write_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_Write_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_Write_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_Write_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_Write_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_Write_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_Write_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_Write_Byte(0xC8,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_Write_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_Write_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_Write_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_Write_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_Write_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_Write_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_Write_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_Write_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_Write_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_Write_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_Write_Byte(0xAF,OLED_CMD); //������ʾ	 
	OLED_Clear();

}


