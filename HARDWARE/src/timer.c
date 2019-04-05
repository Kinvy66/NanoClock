/**
  ******************************************************************************
  * @file    timer.c 
  * @author  Kinvy
  * @version V_x.xx.x
  * @date    20xx-xx-xx
  * @brief   None         
  *          
  ******************************************************************************
  */
#include "timer.h"
#include "led.h"
#include "smg.h"
#include "set.h"

/**
 * @brief  TIM2_Int_Init
 * @param  arr: ��װ��ֵ   psc: Ԥ��Ƶֵ
					 ��ʱ��Ƶ�� Fck_psc/psc+1,  Fck_psc�Ƕ�ʱ����ʱ����Դһ��Ϊ72MHz 
 * @retval None
 */
void TIM3_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	NVIC_InitTypeDef 					NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	TIM_TimeBaseStructure.TIM_Period = arr;  												//������װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc;											//����Ԥ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;					//����ʱ�з�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			//����ģʽ�����ϼ���
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 

}

void TIM2_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		switch(Display)
		{
			case DEFAULT	: Display_Time();	break;
			case DATE			: Display_Date(); break;
			case ALARM		: Display_Alarm();break;
			case SetTime	:	Display_Set_Time(); break;	
			case SetDate	: Display_Set_Date(); break;
		}
//		LED0 = !LED0;
	}
}



