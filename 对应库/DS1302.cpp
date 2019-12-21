#include "DS1302.h"

SYSTEMTIME DS1302Buffer;
bool Flag_Time_Refresh = 1;


void DS1302_InputByte(unsigned char dat)
{
	unsigned char i;

	pinMode(DS1302_IO, OUTPUT);

	for (i = 0; i<8; i++)
	{
		if ((dat & 0x01) == 1)
		{
			DS1302_IO_1;
		}
		else
		{
			DS1302_IO_0;
		}


		DS1302_CLK_0;               //������д������

		delayMicroseconds(2);

		DS1302_CLK_1;

		delayMicroseconds(2);
		dat >>= 1;
	}
}

//****************************************************
//ʵʱʱ�Ӷ�ȡһ�ֽ�(�ڲ�����)
//****************************************************
unsigned char DS1302_OutputByte(void)
{
	unsigned char i;
	unsigned char dat;
	pinMode(DS1302_IO, INPUT);
	for (i = 0; i<8; i++)
	{
		DS1302_CLK_1;                   //�½��ض�������
		delayMicroseconds(2);
		DS1302_CLK_0;
		delayMicroseconds(2);
		dat >>= 1;
		if (digitalRead(DS1302_IO) == HIGH)
			dat |= 0x80;                //���λ��һ
		else
			dat &= 0x7F;                //���λ����       
	}
	return(dat);
}

//****************************************************
//ucAddr: DS1302��ַ, ucData: Ҫд������
//****************************************************
void DS1302_Write(unsigned char ucAddr, unsigned char ucDa)
{
	DS1302_RST_0;
	delayMicroseconds(2);
	DS1302_CLK_0;
	delayMicroseconds(2);
	DS1302_RST_1;
	delayMicroseconds(2);
	DS1302_InputByte(ucAddr);           // ��ַ������ 
	DS1302_InputByte(ucDa);             // д1Byte����
	DS1302_CLK_1;
	delayMicroseconds(2);
	DS1302_RST_0;
	delayMicroseconds(2);
}

//****************************************************
//��ȡDS1302ĳ��ַ������
//****************************************************
unsigned char DS1302_Read(unsigned char ucAddr)
{
	unsigned char ucData;
	DS1302_RST_0;
	delayMicroseconds(2);
	DS1302_CLK_0;
	delayMicroseconds(2);
	DS1302_RST_1;
	delayMicroseconds(2);
	DS1302_InputByte(ucAddr | 0x01);        // ��ַ������ 
	ucData = DS1302_OutputByte();         // ��1Byte����
	DS1302_CLK_1;
	delayMicroseconds(2);
	DS1302_RST_0;
	delayMicroseconds(2);
	return(ucData);
}

//****************************************************
//�Ƿ�д����
//****************************************************
void DS1302_SetProtect(unsigned char flag)
{
	if (flag)
		DS1302_Write(0x8E, 0x10);            //����
	else
		DS1302_Write(0x8E, 0x00);            //������
}

//****************************************************
// ����ʱ�亯��
//****************************************************
void DS1302_SetTime(unsigned char Address, unsigned char Value)
{
	DS1302_SetProtect(0);
	DS1302_Write(Address, ((Value / 10) << 4 | (Value % 10)));
}

//****************************************************
//��ȡʵʱʱ��
//****************************************************
void DS1302_GetTime(SYSTEMTIME *Time)
{
	unsigned char ReadValue;
	ReadValue = DS1302_Read(DS1302_SECOND);
	ReadValue = ((ReadValue & 0x70) >> 4) * 10 + (ReadValue & 0x0F);            //BCD��ת��ʮ����
	if (ReadValue != DS1302Buffer.Second)                                //����һ��״̬��ͬ����λˢ�±�־λ
		Flag_Time_Refresh = 1;
	Time->Second = ReadValue;

	ReadValue = DS1302_Read(DS1302_MINUTE);
	Time->Minute = ((ReadValue & 0x70) >> 4) * 10 + (ReadValue & 0x0F);
	ReadValue = DS1302_Read(DS1302_HOUR);
	Time->Hour = ((ReadValue & 0x70) >> 4) * 10 + (ReadValue & 0x0F);
	ReadValue = DS1302_Read(DS1302_DAY);
	Time->Day = ((ReadValue & 0x70) >> 4) * 10 + (ReadValue & 0x0F);
	ReadValue = DS1302_Read(DS1302_WEEK);
	Time->Week = ((ReadValue & 0x70) >> 4) * 10 + (ReadValue & 0x0F);
	ReadValue = DS1302_Read(DS1302_MONTH);
	Time->Month = ((ReadValue & 0x70) >> 4) * 10 + (ReadValue & 0x0F);
	ReadValue = DS1302_Read(DS1302_YEAR);
	Time->Year = ((ReadValue & 0x70) >> 4) * 10 + (ReadValue & 0x0F);
}

//****************************************************
//��ʼ��
//****************************************************
void DS1302_Init(void)
{
	unsigned char Second;

	pinMode(DS1302_RST, OUTPUT);
	pinMode(DS1302_IO, OUTPUT);
	pinMode(DS1302_CLK, OUTPUT);

	Second = DS1302_Read(DS1302_SECOND);
	if (Second & 0x80)
		DS1302_SetTime(DS1302_SECOND, Second & 0x7f);    //������    
}
//****************************************************
//DS1302����ֹͣ
//****************************************************
void DS1302_ON_OFF(bool FLAG_ON_OFF)
{
	unsigned char Second;
	Second = DS1302_Read(DS1302_SECOND);
	if (FLAG_ON_OFF == 0)
		DS1302_Write(DS1302_SECOND, Second | 0x80);              //�ر���
	else
		DS1302_Write(DS1302_SECOND, Second & 0x7f);              //������
}
