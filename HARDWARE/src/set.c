#include "set.h"
#include "key.h"
#include "exti.h"
#include "timer.h"
#include "rtc.h"
#include "smg.h"
#include "usart.h"
#include "delay.h"

uint8_t Display = DEFAULT;  //��ʾ�Ľ���
uint8_t item = 0;
Time_InitTypeDef Set;

/**
 * @brief  Ĭ����ʾ��ͨ��KEY0,KEY1�л���ʾ����
 * @param  None
 * @retval None
 */
void Switch_Display(void)
{

	if(Display==DEFAULT&&(KEY0_flag!=RESET||KEY1_flag!=RESET||KEY2_flag!=RESET)) //Ĭ�Ͻ�����KEY0��KEY1����
	{
		while(1)
		{
			if(KEY0_flag!=RESET)  
			{				
				KEY0_flag = RESET;
				Display++;
				if(Display==4)
				{
					Display=DEFAULT;
					break;
				}  
			}
//			if(KEY1_flag!=RESET)
//			{
////				Display++;
//				KEY1_flag = RESET;
//			}
			if(KEY2_flag != RESET)
			{
				KEY2_flag = RESET;
				Display=DEFAULT;
				break;
			}
			if(WKUP_flag != RESET||KEY1_flag!=RESET) //��������޹ذ����ı�־λ
			{
				WKUP_flag = RESET;
				KEY1_flag = RESET;
			}
			if(Display==DEFAULT)
				break;
		}
	}
}

/**
 * @brief  �������ʾRTC��ʱ��
 * @param  None
 * @retval None
 */
void Display_Time(void)
{
	static uint8_t smg_wei = 0;
	uint8_t num;
//	Time_InitTypeDef Time;  //ʱ��Ľṹ��
	
//	CNT_to_TIME(RTC_GetCounter(),&Time);

	switch(smg_wei)
	{
		case 0:  num = Time.hour/10; break;//ʱ
		case 1:  num = Time.hour%10; break; 
		case 2:   
		case 5:  num = 17; break;
		case 3:  num = Time.min/10; break; //��
		case 4:  num = Time.min%10; break; 
		case 6:  num = Time.sec/10; break; //��
		case 7:  num = Time.sec%10; break;   
	}
	LED_SMG_Display(num,smg_wei,NoDot);
	smg_wei++;
	if(smg_wei==8) smg_wei = 0;
  
}


/**
 * @brief  ��ʾ����
 * @param  None
 * @retval None
 */
void Display_Date(void)
{
	static uint8_t smg_wei = 0;
	uint8_t num;
	static uint16_t stay = 0;

	switch(smg_wei)
	{
		case 0:  num = Time.w_year/1000; 				break;//��
		case 1:  num = Time.w_year%1000/100; 		break; 
		case 2:  num = Time.w_year%100/10; 			break;
		case 3:  num = Time.w_year%1000%10; 		break; //�˴���BUG %1000�õ�������ݺ���λ
		case 4:  num = Time.w_month/10; 				break; //��
		case 5:  num = Time.w_month%10; 				break; 
		case 6:  num = Time.w_day/10; 					break; //��
		case 7:  num = Time.w_day%10; 					break;   
	}
	if(smg_wei==3||smg_wei==5)
		LED_SMG_Display(num,smg_wei,Dot);
	else
		LED_SMG_Display(num,smg_wei,NoDot);
	smg_wei++;
	if(smg_wei==8) smg_wei = 0;
	stay++;
	if(stay==2500&&Display==DATE)
	{
		stay=0;
		Display = DEFAULT;
	}
}

/**
 * @brief  ��ʾ����
 * @param  None
 * @retval None
 */
void Display_Alarm(void)
{
	
	static uint8_t smg_wei = 0;
	uint8_t num;
	uint8_t Alarm_State = 0;
	static uint16_t stay = 0;

	CNT_to_TIME(RTC_GetALarm(),&Alarm);
	if(Alarm.hour!=0||Alarm.min!=0||Alarm.sec!=0)
	{
		Alarm_State = 1;
	}

	switch(smg_wei)
	{
		case 0:  num = 0; break;  //���ӿ�����رյ�״̬
		case 1:  num = 19; break; 
		case 2:  num = 18;break;
		case 3:  num = Alarm.hour/10; break; //ʱ
		case 4:  num = Alarm.hour%10; break;
		case 5:  num = 17; break;  break;
		case 6:  num = Alarm.min/10; break; //��
		case 7:  num = Alarm.min%10; break;
	}
	
	LED_SMG_Display(num,smg_wei,NoDot);
	smg_wei++;
	if(smg_wei==8) smg_wei = 0;
	stay++;
	if(stay==2500&&Display==ALARM)
	{
		stay=0;
		Display = DEFAULT;
	}
}

/**
 * @brief  �������������״̬��ʾ
 * @param  None
 * @retval None
 */
void Display_Set_Date()
{
	static uint8_t smg_wei = 0;
	static uint16_t blink = 0;
	static uint16_t stay = 0;

	uint8_t num;
	
	blink++;
	{
		switch(smg_wei)
		{
			case 0:  {if(item == SetYear&&blink>300) num = 18; else num = Set.w_year/1000;} break;//��  num=18ʱ�������ʲô������ʾ
			case 1:  {if(item == SetYear&&blink>300) num = 18; else num = Set.w_year%1000/100;} break; 
			case 2:  {if(item == SetYear&&blink>300) num = 18; else num = Set.w_year%100/10;} break;
			case 3:  {if(item == SetYear&&blink>300) num = 18; else num = Set.w_year%1000%10;} break;
			
			case 4:  {if(item == SetMonth&&blink>300) num = 18; else num = Set.w_month/10;} break; //��
			case 5:  {if(item == SetMonth&&blink>300) num = 18; else num = Set.w_month%10;} break; 
			
			case 6:  {if(item == SetDay&&blink>300) num = 18; else num = Set.w_day/10;} break; //��
			case 7:  {if(item == SetDay&&blink>300) num = 18; else num = Set.w_day%10;} break; 

		}
	}
	if(smg_wei==3||smg_wei==5)
		LED_SMG_Display(num,smg_wei,Dot);
	else
		LED_SMG_Display(num,smg_wei,NoDot);
	smg_wei++;
	if(smg_wei==8) smg_wei = 0;
	if(blink == 600) blink = 0;
	stay++;

}
/**
 * @brief  ����ʱ��״̬����ܵ���ʾ
 * @param  None
 * @retval None
 */
void Display_Set_Time(void)
{
	static uint8_t smg_wei = 0;
	static uint16_t blink = 0;
	static uint16_t stay = 0;
	uint8_t num;
	blink++;
	{
		switch(smg_wei)
		{
			case 0:  {if(item == SetHour&&blink>300) num = 18; else num = Set.hour/10; } break;//ʱ 
			case 1:  {if(item == SetHour&&blink>300) num = 18; else num = Set.hour%10;} break; 
			//��ʾ-
			case 2:   
			case 5:  num = 17; break;
			
			case 3:  {if(item == SetMin&&blink>300) num = 18; else num = Set.min/10;} break; //��
			case 4:  {if(item == SetMin&&blink>300) num = 18; else num = Set.min%10;} break; 
			
			case 6:  {if(item == SetSec&&blink>300) num = 18; else num = Set.sec/10;} break; //��
			case 7:  {if(item == SetSec&&blink>300) num = 18; else num = Set.sec%10;} break;   
		}
	}

	LED_SMG_Display(num,smg_wei,NoDot);
	smg_wei++;
	if(smg_wei==8) smg_wei = 0;
	
	if(blink == 600) blink = 0;

	stay++;

}



/**
 * @brief  ����ʱ��
 * @param  None
 * @retval None
 */
void Set_Rtc(void)
{
	
	if(Display==DEFAULT&&WKUP_flag!=RESET)
	{
		WKUP_flag = RESET;
		Set.w_year 	= Time.w_year;
		Set.w_month 	= Time.w_month;
		Set.w_day 		= Time.w_day;
		Set.hour 		= Time.hour;
		Set.min 			= Time.min;
		Set.sec 			= Time.sec;
		Display = SetTime;
		item = SetHour;
		while(1)
		{
			

			if(WKUP_flag!=RESET)
			{
				WKUP_flag = RESET;
				if(item==SetDay)
				{
					item = SetHour;
					Display = DEFAULT;
					break;
				}
				else
					item++;
				if(item > SetSec)
				{
					Display = SetDate;
				}
			}
			
			if(KEY0_flag != RESET) //��
			{
				KEY0_flag = RESET;
				switch(item)
				{
					case SetYear	: 
							{
								if(Set.w_year==2070)
									Set.w_year = 1970;
								else
									Set.w_year++;
							} 	
					break;
					case SetMonth	:	
							{
								if(Set.w_month==12)
									Set.w_month = 1;
								else
									Set.w_month++;
							}	
					break;
					case SetDay		:	
							{
								if(((Set.w_month==1||Set.w_month==3||Set.w_month==5||Set.w_month==7
									||Set.w_month==8||Set.w_month==10||Set.w_month==12)
									&&(Set.w_day==31))																								//����31��
									||((Set.w_month==4||Set.w_month==6||Set.w_month==9||Set.w_month==1)
									&&(Set.w_day==30))																								//����30��
									||((Set.w_year%400==0||(Set.w_year%4==0 &&Set.w_year%100 !=0))
									&&Set.w_month==2&&Set.w_day==29)																	//�������29��
									||(!(Set.w_year%400==0||(Set.w_year%4==0 &&Set.w_year%100 !=0))
									&&Set.w_month==2&&Set.w_day==28)																	//ƽ�����28��
									)
								{
									Set.w_day=1;
								}
								else Set.w_day++;  
							}	
					break;
					case SetHour	:	{if(Set.hour==23) Set.hour=0; else Set.hour++;}			break;
					case SetMin		:	{if(Set.min==59)	Set.min=0;  else Set.min++;}			break;
					case SetSec		:	{if(Set.sec==59)	Set.sec=0;  else Set.sec++;}			break;
				}
					
			}
			
			if(KEY1_flag != RESET)	//��
			{
				KEY1_flag = RESET;
				switch(item)
				{
					case SetYear	: 
							{
								if(Set.w_year==1970)
									Set.w_year = 2070;
								else
									Set.w_year--;
							} 	
					break;
					case SetMonth	:	
							{
								if(Set.w_month==1)
									Set.w_month = 12;
								else
									Set.w_month--;
							}	
					break;
					case SetDay		:	
							{
								if((Set.w_month==1||Set.w_month==3||Set.w_month==5||Set.w_month==7
									||Set.w_month==8||Set.w_month==10||Set.w_month==12)
									&&(Set.w_day==1))
									Set.w_day=31;								//����31��
								else if((Set.w_month==4||Set.w_month==6||Set.w_month==9||Set.w_month==1)
									&&(Set.w_day==1))
									Set.w_day=30;									//����30��
								else if((Set.w_year%400==0||(Set.w_year%4==0 &&Set.w_year%100 !=0))
									&&Set.w_month==2&&Set.w_day==1)
									Set.w_day=29;									//�������29��
								else if(!(Set.w_year%400==0||(Set.w_year%4==0 &&Set.w_year%100 !=0))
									&&Set.w_month==2&&Set.w_day==1)
									Set.w_day=28;								//ƽ�����28��
								else Set.w_day--;
							}	
					break;
					case SetHour	:	{if(Set.hour==0) Set.hour=23; else Set.hour--;}			break;
					case SetMin		:	{if(Set.min==0)	Set.min = 59;	else Set.min--;	}	break;
					case SetSec		:	{if(Set.sec==0)	Set.sec = 59; else Set.sec--;	}		break;
				}
			}
			if(KEY2_flag != RESET)  //д�����õ�ʱ��
			{
				KEY2_flag = RESET;
				RTC_Set(&Set);   //д��Ĵ���
				//����Ĵ�����Ϊ�˷�ֹ�����˳��������������ʾԭ����ʱ��
				//û������Ĵ�����ӳ���ת������
				Time.w_year 	= Set.w_year; 
				Time.w_month 	= Set.w_month;
				Time.w_day 		= Set.w_day;
				Time.hour 		= Set.hour;
				Time.min 			= Set.min;
				Time.sec 			= Set.sec;
				Display=DEFAULT;
				break;
			}
		}
	}
}











