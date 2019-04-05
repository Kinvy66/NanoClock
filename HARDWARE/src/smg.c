/**
  ******************************************************************************
  * @file    smg.c 
  * @author  Kinvy
  * @version V_x.xx.x
  * @date    20xx-xx-xx
  * @brief   None         
  *          
  ******************************************************************************
  */
	
#include "smg.h"
#include "delay.h"
#include "rtc.h"

//0 1 2 3 4 5 6 7 8 9 a b c d e f . -  n
u8 smg_num[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,
							0xf6,0xee,0x3e,0x9c,0x7a,0x9e,0x8e,0x01,0x02,0x00,0xec};

							
/**
 * @brief  LED_SMG_Init
 * @param  None
 * @retval None
 */
void LED_SMG_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //����GPIO��ʼ��ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);  
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //����JTAG
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;  			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_InitStructure);				//��ʼ��74HC595
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOC,&GPIO_InitStructure);				//��ʼ��74HC138
	
	LED_DS   = 0;
	LED_LCLK = 1;
	LED_SCK	 = 0;
	
	LED_DS	 = 0;
	LED_LCLK = 0;
	LED_SCK	 = 0;
	
}

/**
 * @brief  74HC138���������λѡ
 * @param  num:Ҫ��ʾ������ܱ�� 0-7(��8�������)
 * @retval None
 */

void LED_Wei(uint8_t num)
{
  LED_A0=num&0x01;
	LED_A1=(num&0x02)>>1;
	LED_A2=(num&0x04)>>2;
}

/**
 * @brief  74HC595�����������ʾ
 * @param  num:��ʾ����
					 wei:Ҫ��ʾ������ܱ�� 0-7(��8�������)
 * @retval None
 */
void LED_Write_Data(uint8_t duan,uint8_t wei)
{
	u8 i;
//	duan = smg_num[num];
	for( i=0;i<8;i++)//���Ͷ�
	{
		LED_DS=(duan>>i)&0x01;
		LED_SCK=0;
		delay_us(5);
		LED_SCK=1;
	}
    LED_Wei(wei);//��ѡ��λ
}

/**
 * @brief  74HC595���������ˢ����ʾ
 * @param  None
 * @retval None
 */
void LED_Refresh(void)
{
	LED_LCLK=1;
	delay_us(5);
	LED_LCLK=0;
}
/**
 * @brief  �������ʾ��
 * @param  num(0~15),�������ʾ��
					 wei(0~7),��ʾ��λ��
					 dot,��ʾС���� 1:��ʾС���� 0:����ʾС����
 * @retval None
 */
void LED_SMG_Display(uint8_t num,uint8_t wei,uint8_t dot)
{
		LED_Write_Data(smg_num[num]+dot,wei);  //С���������������һλ��1�����Լ�1�Ϳ�ʵ��
		LED_Refresh();
//		delay_ms(200);
}





