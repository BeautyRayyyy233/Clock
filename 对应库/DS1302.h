#ifndef __DS1302_H__
#define __DS1302_H__

#include <Arduino.h>

//DS1302 IO����
#define DS1302_RST 2     //ʵʱʱ�Ӹ�λ������
#define DS1302_IO 3      //ʵʱʱ������������
#define DS1302_CLK 4     //ʵʱʱ��ʱ��������


#define DS1302_RST_0 digitalWrite(DS1302_RST,LOW)
#define DS1302_RST_1 digitalWrite(DS1302_RST,HIGH)

#define DS1302_IO_0 digitalWrite(DS1302_IO,LOW)
#define DS1302_IO_1 digitalWrite(DS1302_IO,HIGH)

#define DS1302_CLK_0 digitalWrite(DS1302_CLK,LOW)
#define DS1302_CLK_1 digitalWrite(DS1302_CLK,HIGH)


//�궨��
#define DS1302_SECOND   0x80
#define DS1302_MINUTE   0x82
#define DS1302_HOUR     0x84 
#define DS1302_DAY      0x86
#define DS1302_MONTH    0x88
#define DS1302_WEEK     0x8a
#define DS1302_YEAR     0x8c 


typedef struct __SYSTEMTIME__
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Week;
	unsigned char Day;
	unsigned char Month;
	unsigned char  Year;
}SYSTEMTIME;    //�����ʱ������


//�������߱�������
extern void DS1302_InputByte(unsigned char dat);
extern unsigned char DS1302_OutputByte(void);
extern void DS1302_Write(unsigned char ucAddr, unsigned char ucDa);
extern unsigned char DS1302_Read(unsigned char ucAddr);
extern void DS1302_SetProtect(unsigned char flag);
extern void DS1302_SetTime(unsigned char Address, unsigned char Value);
extern void DS1302_GetTime(SYSTEMTIME* Time);
extern void DS1302_Init(void);
extern void DS1302_ON_OFF(bool FLAG_ON_OFF);


extern SYSTEMTIME DS1302Buffer;
extern bool Flag_Time_Refresh;

#endif#pragma once
