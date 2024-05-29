/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   WAV������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/**
  ******************************************************************************
  *                              ͷ�ļ�
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "Bsp/usart/bsp_debug_usart.h"
#include "Bsp/systick/bsp_SysTick.h"
#include "Bsp/wm8978/bsp_wm8978.h" 
#include "App/mp3Player.h"
#include "ff.h" 

/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
FATFS fs;											/* Work area (file system object) for logical drives */	
/**
  ******************************************************************************
  *                                ������
  ******************************************************************************
  */

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	FRESULT result; 
  
	/*  NVIC�ж����ȼ���ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  	
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	Debug_USART_Config();
	
	/* ����SD���ļ�ϵͳ */
	result = f_mount(&fs,"0:",1);  //�����ļ�ϵͳ
	if(result!=FR_OK)
	{
		printf("\n SD���ļ�ϵͳ����ʧ��\n");
		while(1);
	}
	
	/* ��ʼ��ϵͳ�δ�ʱ�� */
	SysTick_Init();	
	printf("MP3������\n");
	
	/* ���WM8978оƬ���˺������Զ�����CPU��GPIO */
	if (wm8978_Init()==0)
	{
		printf("��ⲻ��WM8978оƬ!!!\n");
		while (1);	/* ͣ�� */
	}
	printf("��ʼ��WM8978�ɹ�\n");	
	
	while(1)
	{		
		mp3PlayerDemo("0:/mp3/�Ź���-����֮��.mp3");
		mp3PlayerDemo("0:/mp3/�Ź���-ȫ������.mp3");
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
