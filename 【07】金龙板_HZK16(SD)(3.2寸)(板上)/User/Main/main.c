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
** Descriptions:        编写示例代码
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
	*名称：void Delay(void)
	*参数：无
	*返回：无
	*功能：简单延时
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
	LCD_Init();			 /* LCD初始化 */
	LCD_Clear(BLACK);
	USART_Configuration();
	printf("\r\n Hello world! \r\n");
	if(!SD_Init())
	{
		LCD_ShowString(0, 300, "SD Init OK!", BLACK, BROWN);        /*SD卡初始化OK*/
	}
	else LCD_ShowString(0, 300, "SD Init Failure!", BLACK, BROWN); /*SD卡初始化OK*/


	/*--------显示汉字 从字库----------*/
	GUI_Put(40,10, "--深圳旺宝电子科技--", RED, BLACK);	// BLUE
  GUI_Put(45,40, "**  红龙开发板  **", RED, BLACK);
	GUI_Put(37,70, " ^-^汉字字库实验^-^ ", WHITE, BLACK);		
	GUI_Put(32, 120, "感谢您的支持！", CYAN, BLACK);	
  GUI_Put(32, 136, "您的支持是我们前行的动力！", CYAN, BLACK);	
	
	GUI_Put(32, 190, "蓦然回首，那人却在灯火阑珊处！", MAGENTA, BLACK);		
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
