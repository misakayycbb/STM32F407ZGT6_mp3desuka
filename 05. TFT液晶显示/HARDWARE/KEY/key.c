#include "stm32f4xx.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
	
u8  keyvalue=0;	
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��PORTA,ʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
}
//����������
//���ذ���ֵ
void KEY_Scan(void)
{	 	  
	if(KEY0==0||KEY1==0||KEY2==0)
	{
		delay_ms(10);//ȥ���� 
		if(KEY0==0)
		{keyvalue= 3;}
		else 
		if(KEY1==0)
		{keyvalue=1;}
		else 
		if(KEY2==0)
		{keyvalue= 2;}
	}

}
