/****************************Copyright(c)*****************************
**                      http://www.openmcu.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2012-07-23
** Last Version:        V1.00
** Descriptions:        
**
**----------------------------------------------------------------------------------------------------
** Created by:          openmcu
** Created date:        2012-08-27
** Version:             V1.00
** Descriptions:        ��дʾ������
**
**----------------------------------------------------------------------------------------------------
** Modified by:         ZQJ
** Modified date:        
** Version:            
** Descriptions:        
**																		 
*****************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x.h"
#include "107_32_Driver_IO16.h"
#include "sddriver.h"
#include "ff.h"
#include "hw_config.h"




/* Private functions ---------------------------------------------------------*/

/**									 _
	*���ƣ�void Delay(void)
	*��������
	*���أ���
	*���ܣ�����ʱ
**/
void Delay(void)
{
	int x,y;
	for(x=1000;x>0;x--)
		for(y=1000;y>0;y--);

}

							   
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

int main(void)
{
	/* Add your application code here
	*/
	LED_Configuration();
	LCD_Init();			 /* LCD��ʼ�� */
	LCD_Clear(BLACK);
	USART_Configuration();
	printf("\r\n Hello world! \r\n");
	if(!SD_Init())
	{
		LCD_ShowString(0, 300, "SD Init OK!", BLACK, BROWN);        /*SD����ʼ��OK*/
	}
	else LCD_ShowString(0, 300, "SD Init Failure!", BLACK, BROWN); /*SD����ʼ��OK*/


	/*--------��ʾ���� ���ֿ�----------*/
	GUI_Put(40,10, "--�����������ӿƼ�--", RED, BLACK);	// BLUE
  GUI_Put(45,40, "**  ����������  **", RED, BLACK);
	GUI_Put(37,70, " ^-^�����ֿ�ʵ��^-^ ", WHITE, BLACK);		
	GUI_Put(32, 120, "��л����֧�֣�", CYAN, BLACK);	
  GUI_Put(32, 136, "����֧��������ǰ�еĶ�����", CYAN, BLACK);	
	
	GUI_Put(32, 190, "��Ȼ���ף�����ȴ�ڵƻ���ɺ����", MAGENTA, BLACK);		
	while(1)
	{
		LED1(1);
		Delay();
		LED1(0);
		Delay();
	}	
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  * @note   #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */

  USART_SendData(USARTx, (uint8_t) ch);   

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
