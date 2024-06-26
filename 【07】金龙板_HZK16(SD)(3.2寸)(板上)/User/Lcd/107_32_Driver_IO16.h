#ifndef __107_32_Driver_IO16_H_
#define __107_32_Driver_IO16_H_
#include "hw_config.h"

#define LCD_CS_PIN			GPIO_Pin_6	
#define LCD_RS_PIN			GPIO_Pin_13
#define LCD_WR_PIN			GPIO_Pin_14
#define LCD_RD_PIN			GPIO_Pin_15

#define RCC_LCD_DATA_GPIO		RCC_APB2Periph_GPIOE
#define RCC_LCD_CONTROL_GPIO	RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
#define LCD_DATA_GPIO			GPIOE  

#define MAX_HZ_POSX 224
#define MAX_HZ_POSY 304

/*Private define-------------------------*/
#define POINT_COLOR RED
#define BACK_COLOR	0xFFFF
/*--------16位颜色值---------------*/
#define WHITE        0xFFFF
#define BLACK        0x0000	  
#define BLUE         0x001F  
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN        0x07E0
#define CYAN         0x7FFF
#define YELLOW       0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define LGRAY 			 0XC618 //浅灰色

/*----------函数声明------------*/
/* 刷屏 */
void LCD_Clear(uint16_t Color);

void LCD_Fill(uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend, uint16_t colour);
void LCD_DrawLine(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend);
void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);
void LCD_DrawRectangle(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend);

void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end);
/*显示字符 包括中英文*/
void LCD_ShowString(u16 x0, u16 y0, u8 *pcStr, u16 PenColor, u16 BackColor);
void GUI_Put(u16 x, u16 y, u8 *str,u32 PenColor, u32 BackColor);
/*显示图片 gImage取模 bmp格式*/
void LCD_DrawPicture(u16 StartX,u16 StartY,u16 Xend,u16 Yend,u8 *pic);


void LCD_Configuration(void);
void LCD_Init(void);


#endif
