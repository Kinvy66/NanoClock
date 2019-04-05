/**
  ******************************************************************************
  * @file    rtc.c 
  * @author  Kinvy
  * @version V_x.xx.x
  * @date    20xx-xx-xx
  * @brief   None         
  *          
  ******************************************************************************
  */

#include "rtc.h"
#include "delay.h"
#include "led.h"
#include "usart.h"

#define SEC_PER_DAY		86400UL
#define SEC_PER_HOU		3600UL
#define SEC_PER_MIN		60

Time_InitTypeDef Time;
Time_InitTypeDef Alarm;

const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};



/**
 * @brief  RTC NVIC
 * @param  None
 * @retval None
 */
static void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


/**
 * @brief  Init RTC 
 * @param  None
 * @retval 0,Successful;  1,failed 
 */
uint8_t RTC_Init(void)   
{
	uint8_t temp=0;
  Time_InitTypeDef Time_InitStruct;	//�����ṹ��
	
	Time_InitStruct.w_year 	= 2019;
	Time_InitStruct.w_month 	= 1;
	Time_InitStruct.w_day  	= 27;
	Time_InitStruct.hour 		= 14;
	Time_InitStruct.min 			= 40;
	Time_InitStruct.sec 			= 00;
	

                 
	/* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)		//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{
		/* Reset Backup Domain */
		BKP_DeInit();

		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);
		/* Wait till LSE is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<250)
		{
			temp++;
			delay_ms(10);
		}
			
		if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������

		/* Select LSE as RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
			
		RTC_EnterConfigMode();/// ��������	

		/* Set RTC prescaler: set RTC period to 1sec */
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		RTC_Set(&Time_InitStruct);  //����ʱ��	
		RTC_ExitConfigMode(); //�˳�����ģʽ  
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	}
	else//ϵͳ������ʱ
	{

		RTC_WaitForSynchro();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);	//ʹ��RTC���жϺ������ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}
	RTC_NVIC_Config();//RCT�жϷ�������		    				     
	RTC_Get();//����ʱ��	
	return 0;
}

/**
 * @brief  None
 * @param  None
 * @retval None
 */
uint8_t RTC_Set(Time_InitTypeDef* Time_InitStruct)//����ʱ��			 
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(TIME_to_CNT(Time_InitStruct));	//����RTC��������ֵ

}

/**
 * @brief  ��������
 * @param  Time_InitStruct: ʱ��ṹ��
					 mode  1:һ��;0:ÿ��
 * @retval None
 */
uint8_t RTC_Alarm_Set(Time_InitTypeDef* Time_InitStruct,uint8_t mode)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetAlarm(TIME_to_CNT(Time_InitStruct));
		
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	
	return 0;	 
}


/**
 * @brief  ����ʱ�� 
 * @param  None
 * @retval None
 */
uint8_t RTC_Get(void)        
{
	CNT_to_TIME(RTC_GetCounter(),&Time);
	return 0;
}

/**
 * @brief  ��ȡ��ǰ���õ�����
 * @param  None
 * @retval None
 */
uint32_t RTC_GetALarm(void)
{
  uint16_t tmp = 0;
  tmp = RTC->ALRL;
  return (((uint32_t)RTC->ALRH << 16 ) | tmp) ;
}




//RTC�жϷ�����
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//�����ж�
	{							
		RTC_Get();//����ʱ��   
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
	{
		LED0 = 0;
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	  	
		RTC_Get();//����ʱ��   
  } 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//�������ж�
	RTC_WaitForLastTask();	  	    						 	   	 
}





/**
 * @brief  �ж��Ƿ�Ϊ����
 * @param  year:���
 * @retval 1:����		0:ƽ��
 */
static uint8_t Is_Leap_Year(uint16_t year)
{
	uint8_t rev;
	(year%400==0||(year%4==0 &&year%100 !=0))? (rev=1) : (rev=0); 
	return rev;
}


/**
  * @brief  ʱ���(����������ֵ)ת��ʱ��
  * @param  cnt: count value
	* @param  Time_InitTypeDef: Time structure
  * @retval 0:Successful;  other:failed 
  */
uint8_t CNT_to_TIME(uint32_t cnt,Time_InitTypeDef* Time_InitStruct)  
{
	uint16_t days;
	
	days = cnt / (24*3600);  //��������
	
	//����ʱ����
	Time_InitStruct->hour = cnt%(24*3600)/3600;  
	Time_InitStruct->min = cnt%(24*3600)%3600/60;
	Time_InitStruct->sec = cnt%(24*3600)%3600%60 ;
	
	//��ʼ��Ϊ1970.1.1
	Time_InitStruct->w_year =1970;   	
	Time_InitStruct->w_month=1;
	Time_InitStruct->w_day	=1;
	Time_InitStruct->week 	= 4;
	if(days > 0) //����һ��
	{
		Time_InitStruct->week=(days+4)%7+1;	//�������ڣ���ͨ�������㷨�Ż�
		
		while(days >= 365)  //�������
		{
			if(Is_Leap_Year(Time_InitStruct->w_year))
			{
				if(days>=366) days -= 366;
				else {Time_InitStruct->w_year++; break;}
			}
			else days -= 365;  //ƽ��
			
			Time_InitStruct->w_year++;	
		}
//		printf("year-days:%d\n",days);
		while(days>=28)//�����·�
		{
//			printf("month:%d\n",Time_InitStruct->w_month);
			if(Is_Leap_Year(Time_InitStruct->w_year)&& (Time_InitStruct->w_month==2))
			{
				if(days>=29) days -= 29;
				else break;
			}
			else
			{
				if(days>=mon_table[Time_InitStruct->w_month-1])
					days -= mon_table[Time_InitStruct->w_month-1];
				else 
					{
						Time_InitStruct->w_month++;
						break;
					}
			}
			Time_InitStruct->w_month++;
		}
//		Time_InitStruct->w_month;
		if(days == 0)
			Time_InitStruct->w_day=mon_table[Time_InitStruct->w_month-1];
		else
			Time_InitStruct->w_day = days; //��������
	
	}
	
	return 0;
}

/**
 * @brief  ʱ��ת�ɼ���ֵ
 * @param  ʱ��ṹ��
 * @retval ��ǰʱ���ʱ���
 */
uint32_t TIME_to_CNT(Time_InitTypeDef* Time_InitStruct)
{
	uint16_t days = 0;
	uint16_t i;
	
	//�޶�ʱ������÷�Χ
	if(Time_InitStruct->w_year<1970||Time_InitStruct->w_year>2070) return 1;	
	
	for(i = 1970;i < Time_InitStruct->w_year; i++)	//���������
	{
		if(Is_Leap_Year(i))
			days += 366;
		else days += 365;
	}
	for(i = 1;i < Time_InitStruct->w_month; i++) //ʣ���µ�����
	{
		days += mon_table[i-1];
	}

	if(Is_Leap_Year(Time_InitStruct->w_year)&&(Time_InitStruct->w_month>2))
		days += 1;
	
	days += Time_InitStruct->w_day; //ʣ���յ�����
	

	return (days*SEC_PER_DAY)+(Time_InitStruct->hour*SEC_PER_HOU)
					+(Time_InitStruct->min*SEC_PER_MIN)+Time_InitStruct->sec;
}








