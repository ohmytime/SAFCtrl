#include "stm32f1xx_hal.h"
#include "main.h"
#include "string.h"

#ifndef __UI_H__
#define __UI_H__

/*
经过测试J204A(也称2004A,每行20字符，显示4行)液晶屏和1602液晶屏的
使用方法几乎是一样的，不过也是有区别的，在1602液晶屏中，用来寄存待显
示字符代码的DDRAM共有80个字节，而J204A能够一次性显示的字符是80字
节，这里不是巧合，即J204A只是将LCD1602的DDRAM的80个字符全部显示出
来而已。
  
第一行首地址        :0x80
第二行首地址        :0x80+0x40
第三行首地址        :0x80+20
第四行首地址        :0x80+0x40+20

可以注意到，第一行和第三行、第二行和第四行的地址是分别连续的。 经过
实验验证，可以得出这样的结论:J204A是将LCD1602的DDRAM的0x00~0x13地址
的数据映射显示在J204A的第一行，LCD1602的DDRAM的0x14~0x27地址的数据映
射显示在第三行 ； LCD1602的DDRAM的0x40~0x53地址的数据映射显示在J204A
的第二行，将LCD1602的DDRAM的0x54~0x67地址的数据映射显示在J204A的第四行。
*/
// for LCD 2004
#define LCD2004_ROW0        0
#define LCD2004_ROW1        1
#define LCD2004_ROW2        2
#define LCD2004_ROW3        3
#define LCD2004_MIN_ROW LCD2004_ROW0

#define LCD2004_ROW_LENGHT        0X14 //20

#define LCD2440_MAX_ROW        LCD2004_ROW3
#define LCD2004_ROW0_ADDRESS_START (0X80)
#define LCD2004_ROW1_ADDRESS_START (0X80+0X40)
#define LCD2004_ROW2_ADDRESS_START (LCD2004_ROW0_ADDRESS_START+LCD2004_ROW_LENGHT)
#define LCD2004_ROW3_ADDRESS_START (LCD2004_ROW1_ADDRESS_START+LCD2004_ROW_LENGHT)


void Delay(uint16_t time);
void LCDSetBackLight(uint8_t Data);
void LCDEN(void);
void LCDCmd(uint8_t Cmd);
void LCDData(uint8_t Data);
void LCDClr(void);
void LCDClr1(void);
void LCDOn(void);
void LCDInit(void);
void LCDXY(uint8_t x, uint8_t y);
void LCDXYChar(uint8_t x, uint8_t y, const char c);
void LCDXYStr(uint8_t x, uint8_t y, const char *str);
void LCDFullStr(const char *str);
void LCDXYStrLen(uint8_t x, uint8_t y, const char *str, uint8_t nLen, uint8_t bLeftAlign);
void LCDXYIntLen(uint8_t x, uint8_t y, int32_t n, uint8_t nLen);
void LCDXYUIntLenZP(uint8_t x, uint8_t y, uint32_t n, uint8_t nLen);

#endif