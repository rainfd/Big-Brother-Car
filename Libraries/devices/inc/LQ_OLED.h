#include "common.h"
//#include "includes.h"

#ifndef _Oled_H
#define _Oled_H

//#include "includes.h"
#include "common.h"
#include "gpio.h"


#define byte char//uint8_t
#define word uint16_t
//#define GPIO_PIN_MASK      0x1Fu    //0x1f=31,限制位数为0--31有效
//#define GPIO_PIN(x)        (((1)<<(x & GPIO_PIN_MASK)))  //把当前位置1

//#define LCD_Port_Init()            GPIO_INIT(PORTC,16, GPO,0);   \
//                                   GPIO_INIT(PORTC,17, GPO,0);   \
//                                   GPIO_INIT(PORTC,18, GPO,0);   \
//                                   GPIO_INIT(PORTC,19, GPO,0);

#define LCD_Port_Init()            GPIO_QuickInit(HW_GPIOC, 16, kGPIO_Mode_OPP);  \
                                   GPIO_QuickInit(HW_GPIOC, 17, kGPIO_Mode_OPP);  \
                                   GPIO_QuickInit(HW_GPIOC, 18, kGPIO_Mode_OPP);  \
                                   GPIO_QuickInit(HW_GPIOC, 19, kGPIO_Mode_OPP);




#define LCD_DC_HIGH     PCout(19) = 1;//GPIO_SET(PORTC, 19, 1)
#define LCD_DC_LOW      PCout(19) = 0;//GPIO_SET(PORTC, 19, 0)

#define LCD_SCL_HIGH    PCout(16) = 1;//GPIO_SET(PORTC, 16, 1)
#define LCD_SCL_LOW     PCout(16) = 0;//GPIO_SET(PORTC, 16, 0)

#define LCD_SDA_HIGH    PCout(17) = 1;//GPIO_SET(PORTC, 17, 1)
#define LCD_SDA_LOW     PCout(17) = 0;//GPIO_SET(PORTC, 17, 0)

#define LCD_RST_HIGH    PCout(18) = 1;//GPIO_SET(PORTC, 18, 1)
#define LCD_RST_LOW     PCout(18) = 0;//GPIO_SET(PORTC, 18, 0)


 extern byte longqiu96x64[768];
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_Set_Pos(byte x, byte y);
 void LCD_WrDat(byte data);
 void LCD_P6x8Str(byte x,byte y,byte ch[]);
 void LCD_P8x16Str(byte x,byte y,byte ch[]);
 void LCD_P14x16Str(byte x,byte y,byte ch[]);
 void LCD_Print(byte x, byte y, byte ch[]);
 void LCD_Print_Num(byte x, byte y, int num, uint8_t len);
 void LCD_PutPixel(byte x,byte y);
 void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]);
 void LCD_Fill(byte dat);
#endif