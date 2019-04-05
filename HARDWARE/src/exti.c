#include "exti.h"
#include "key.h"
#include "delay.h"
#include "led.h"

uint8_t KEY0_flag=0;
uint8_t KEY1_flag=0;
uint8_t KEY2_flag=0;
uint8_t WKUP_flag=0;


/**
 * @brief  �ⲿ�жϳ�ʼ��
 * @param  None
 * @retval None
 */
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
		
	KEY_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	
	//GPIOC.8  KEY0  �½��ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//GPIOC.9  KEY1  �½��ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_Init(&EXTI_InitStructure);
	
	//GPIOD.2  KEY2  �½��ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//GPIOA.0  WK_UP  �����ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
	//NVIC����
	//�ⲿ�ж�0���� WK_UP
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  						//ѡ���ⲿ�ж�0��ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);

	//�ⲿ�ж�0���� KEY2
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;  						//ѡ���ⲿ�ж�2��ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
	
	//�ⲿ�ж�0���� KEY0,KEY1
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  						//ѡ���ⲿ�ж�5-9��ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
		
}
	



//�ⲿ�жϷ�����
void EXTI0_IRQHandler(void)   //WK_UP
{
	delay_ms(10);    //����
	if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
	{				 
//		LED0=!LED0;
		WKUP_flag = SET;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line0);  //���LINE0�ϵ��жϱ�־λ  
}

//KEY0,KEY1
void EXTI9_5_IRQHandler(void)	
{
	delay_ms(10);    //����
	if(EXTI_GetFlagStatus(EXTI_Line8) != RESET)
	{				 
//		LED2=!LED2;
		KEY0_flag = SET;
	}
		if(EXTI_GetFlagStatus(EXTI_Line9) != RESET)
	{				 
//		LED3=!LED3;
		KEY1_flag = SET;
	}
	EXTI_ClearITPendingBit(EXTI_Line8|EXTI_Line9);  //���LINE8.9�ϵ��жϱ�־λ
}

//KEY2
void EXTI2_IRQHandler(void)		
{
	delay_ms(10);    //����
	if(EXTI_GetFlagStatus(EXTI_Line2) != RESET)
	{				 
//		LED1=!LED1;
		KEY2_flag = SET;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE2�ϵ��жϱ�־λ
}
	


	





