#include "led.h"

/**
  * @brief ������ʼ��
  *         
  * @param  None
  *         
  * @retval None
  */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //����GPIO��ʼ��ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);  //ʹ��GPIOC��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  			//LED����������PC13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//GPIO��ģʽΪ�������
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_50MHz;	//������ٶ�Ϊ50MHz
	GPIO_Init(GPIOC,&GPIO_InitStructure);				//��ʼ��PC13
	
	LED0 = 1;
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
}







