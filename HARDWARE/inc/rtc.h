#ifndef __RTC_H
#define __RTC_H	    

#include "stm32f10x.h"
//#include "calendar.h"
//ʱ��ṹ��
 typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 w_year;
	vu8  w_month;
	vu8  w_day;
	vu8  week;		 
} Time_InitTypeDef;	

extern Time_InitTypeDef Time;
extern Time_InitTypeDef Alarm;

uint8_t RTC_Init(void);        																					//��ʼ��RTC,����0,ʧ��;1,�ɹ�;
uint8_t RTC_Get(void);         																					//����ʱ�� 
uint32_t RTC_GetALarm(void);																						//��ȡ���ӼĴ�����ֵ
uint8_t RTC_Set(Time_InitTypeDef* Time_InitStruct);											//����ʱ��			 
uint8_t RTC_Alarm_Set(Time_InitTypeDef* Time_InitStruct,uint8_t mode);  //��������		 
//��Ӳ���޹ص�ʱ��ת������
uint8_t CNT_to_TIME(uint32_t cnt,Time_InitTypeDef* Time_InitStruct);  	//ʱ���(����������ֵ)ת��ʱ��
uint32_t TIME_to_CNT(Time_InitTypeDef* Time_InitStruct);							  //ʱ��ת�ɼ���ֵ


#endif



