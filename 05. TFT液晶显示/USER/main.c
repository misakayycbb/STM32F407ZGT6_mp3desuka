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
/*���ڲ��Ը���Һ���ĺ���*/
void LCD_Test(void)
{
		/*��ʾ��ʾ����*/
		static uint8_t testCNT = 0;	
		char dispBuff[100];
		testCNT++;	
		LCD_SetFont(&Font8x16);
		LCD_SetColors(RED,WHITE);
		ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
		/********��ʾ�ַ���ʾ��*******/
		ILI9341_DispStringLine_EN(LINE(0),"BH 3.2 inch LCD para:");
		ILI9341_DispStringLine_EN(LINE(1),"Image resolution:240x320 px");
		ILI9341_DispStringLine_EN(LINE(2),"ILI9341 LCD driver");
		ILI9341_DispStringLine_EN(LINE(3),"XPT2046 Touch Pad driver");
		/********��ʾ����ʾ��*******/
		LCD_SetFont(&Font16x24);
		LCD_SetTextColor(GREEN);
		/*ʹ��c��׼��ѱ���ת�����ַ���*/
		sprintf(dispBuff,"Count : %d ",testCNT);
		LCD_ClearLine(LINE(4));	/* ����������� */
		/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
		ILI9341_DispStringLine_EN(LINE(4),dispBuff);
		/*******��ʾͼ��ʾ��******/
		LCD_SetFont(&Font24x32);
		/* ��ֱ�� */
		LCD_ClearLine(LINE(4));/* ����������� */
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
		ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* ��������ʾȫ�� */
	   /*������*/
		LCD_ClearLine(LINE(4));	/* ����������� */
		LCD_SetTextColor(BLUE);
		ILI9341_DispStringLine_EN(LINE(4),"Draw Rect:");
		LCD_SetTextColor(RED);
		ILI9341_DrawRectangle(50,200,100,30,1);
		LCD_SetTextColor(GREEN);
		ILI9341_DrawRectangle(160,200,20,40,0);
		LCD_SetTextColor(BLUE);
		ILI9341_DrawRectangle(170,200,50,20,1);
		Delay(0xFFFFFF);
		ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* ��������ʾȫ�� */
		/* ��Բ */
		LCD_ClearLine(LINE(4));	/* ����������� */
		LCD_SetTextColor(BLUE);
		ILI9341_DispStringLine_EN(LINE(4),"Draw Cir:");
		LCD_SetTextColor(RED);
		ILI9341_DrawCircle(100,200,20,0);
		LCD_SetTextColor(GREEN);
		ILI9341_DrawCircle(100,200,10,1);
		LCD_SetTextColor(BLUE);
		ILI9341_DrawCircle(140,200,20,0);
		Delay(0xFFFFFF);
		ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* ��������ʾȫ�� */
}
 int main(void)
 {	 
 	u8 t=0;	
	delay_init(168);	    	 //��ʱ������ʼ��	  
	ILI9341_Init (); 
    ILI9341_GramScan ( 6 );	    
	while(1)
	{	
	     LCD_Test(); 
	} 
 }
 
 

