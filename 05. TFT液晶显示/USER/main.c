#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	

static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}
/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
		/*演示显示变量*/
		static uint8_t testCNT = 0;	
		char dispBuff[100];
		testCNT++;	
		LCD_SetFont(&Font8x16);
		LCD_SetColors(RED,WHITE);
		ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		/********显示字符串示例*******/
		ILI9341_DispStringLine_EN(LINE(0),"BH 3.2 inch LCD para:");
		ILI9341_DispStringLine_EN(LINE(1),"Image resolution:240x320 px");
		ILI9341_DispStringLine_EN(LINE(2),"ILI9341 LCD driver");
		ILI9341_DispStringLine_EN(LINE(3),"XPT2046 Touch Pad driver");
		/********显示变量示例*******/
		LCD_SetFont(&Font16x24);
		LCD_SetTextColor(GREEN);
		/*使用c标准库把变量转化成字符串*/
		sprintf(dispBuff,"Count : %d ",testCNT);
		LCD_ClearLine(LINE(4));	/* 清除单行文字 */
		/*然后显示该字符串即可，其它变量也是这样处理*/
		ILI9341_DispStringLine_EN(LINE(4),dispBuff);
		/*******显示图形示例******/
		LCD_SetFont(&Font24x32);
		/* 画直线 */
		LCD_ClearLine(LINE(4));/* 清除单行文字 */
		LCD_SetTextColor(BLUE);
		ILI9341_DispStringLine_EN(LINE(4),"Draw line:");
		LCD_SetTextColor(RED);
		ILI9341_DrawLine(50,170,210,230);  
		ILI9341_DrawLine(50,200,210,240);
		LCD_SetTextColor(GREEN);
		ILI9341_DrawLine(100,170,200,230);  
		ILI9341_DrawLine(200,200,220,240);
		LCD_SetTextColor(BLUE);
		ILI9341_DrawLine(110,170,110,230);  
		ILI9341_DrawLine(130,200,220,240);
		Delay(0xFFFFFF);
		ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */
	   /*画矩形*/
		LCD_ClearLine(LINE(4));	/* 清除单行文字 */
		LCD_SetTextColor(BLUE);
		ILI9341_DispStringLine_EN(LINE(4),"Draw Rect:");
		LCD_SetTextColor(RED);
		ILI9341_DrawRectangle(50,200,100,30,1);
		LCD_SetTextColor(GREEN);
		ILI9341_DrawRectangle(160,200,20,40,0);
		LCD_SetTextColor(BLUE);
		ILI9341_DrawRectangle(170,200,50,20,1);
		Delay(0xFFFFFF);
		ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */
		/* 画圆 */
		LCD_ClearLine(LINE(4));	/* 清除单行文字 */
		LCD_SetTextColor(BLUE);
		ILI9341_DispStringLine_EN(LINE(4),"Draw Cir:");
		LCD_SetTextColor(RED);
		ILI9341_DrawCircle(100,200,20,0);
		LCD_SetTextColor(GREEN);
		ILI9341_DrawCircle(100,200,10,1);
		LCD_SetTextColor(BLUE);
		ILI9341_DrawCircle(140,200,20,0);
		Delay(0xFFFFFF);
		ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */
}
 int main(void)
 {	 
 	u8 t=0;	
	delay_init(168);	    	 //延时函数初始化	  
	ILI9341_Init (); 
    ILI9341_GramScan ( 6 );	    
	while(1)
	{	
	     LCD_Test(); 
	} 
 }
 
 

