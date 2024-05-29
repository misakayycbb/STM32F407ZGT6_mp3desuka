#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "usmart.h"   
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	 
#include "wm8978.h"	 
#include "audioplay.h"	

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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	usmart_dev.init(84);		//初始化USMART
 	ILI9341_Init ();				//LCD初始化  
 	KEY_Init();					//按键初始化  
	W25QXX_Init();				//初始化W25Q128
	WM8978_Init();				//初始化WM8978
	WM8978_HPvol_Set(40,40);	//耳机音量设置
	WM8978_SPKvol_Set(50);		//喇叭音量设置
	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	exfuns_init();				//为fatfs相关变量申请内存  
  	f_mount(fs[0],"0:",1); 		//挂载SD卡  
//	POINT_COLOR=RED;      
	//while(font_init()) 			//检查字库
	//{	    
		//LCD_ShowString(30,50,200,16,16,"Font Error!");
		//delay_ms(200);				  
		//LCD_Fill(30,50,240,66,WHITE);//清除显示	     
		//delay_ms(200);				  
	//}  	 
	//POINT_COLOR=RED;      
 	//Show_Str(60,50,200,16,"Explorer STM32F4开发板",16,0);				    	 
				    	 
	//Show_Str(60,110,200,16,"2014年5月24日",16,0);
	//Show_Str(60,130,200,16,"KEY0:NEXT   KEY2:PREV",16,0); 
	//Show_Str(60,150,200,16,"KEY_UP:PAUSE/PLAY",16,0);
	while(1)
	{ 
		LCD_Test();
		audio_play();
	} 
}




