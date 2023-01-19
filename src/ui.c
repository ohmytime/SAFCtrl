#include "ui.h"
////////////////////////////////////////////////////////////
// LCD utility

void Delay(uint16_t time)
{
	while (time--)
	{
		for (int i = 0; i < 10; i++)
			;
	}
}

void LCDSetBackLight(uint8_t Data)
{
	//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Data);
}

void LCDEN(void) // Generate LCD EN pulse
{
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
	Delay(2);
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
}

void LCDCmd(uint8_t Cmd) // Write LCD command
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (Cmd & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (Cmd & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (Cmd & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (Cmd & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	LCDEN();
	HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (Cmd & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (Cmd & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (Cmd & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (Cmd & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	LCDEN();
	Delay(50);
}

void LCDData(uint8_t Data) // Write LCD data
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (Data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (Data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (Data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (Data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	LCDEN();
	HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (Data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (Data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (Data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (Data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	LCDEN();
	Delay(50);
}

void LCDClr(void)
{
	LCDCmd(0x01); // Clear LCD
	HAL_Delay(5);
}

void LCDClr1(void)
{
	LCDXYStr(0, 1, "                "); // Clear LCD's 2nd row
}

void LCDOn(void)
{
    LCDSetBackLight(255);

	// if (nBacklightKeep && bLCDOff)
	// {
	// 	LCDSetBackLight(255);
	// 	bLCDOff = false;
	// }
	// nBacklightTimer = HAL_GetTick();
}

void LCDInit(void)
{
	LCDCmd(0x33); 
	HAL_Delay(5);
	LCDCmd(0x32);
	HAL_Delay(5);
	LCDCmd(0x28); // 4 bits interface
	HAL_Delay(5);
	LCDSetBackLight(255);
	LCDCmd(0x28);
	HAL_Delay(5);

	LCDCmd(0x28);
	HAL_Delay(5);

	LCDCmd(0x08); // LCD off
	Delay(50);
	LCDClr();	  // Clear LCD
	LCDCmd(0x06); // Auto increment
	Delay(50);
	LCDCmd(0x0c); // LCD on
	HAL_Delay(15);
} // void LCDInit(void)

void LCDXY(uint8_t column, uint8_t row) // x:0-15, y:0-1
{	
	// 1602
	//LCDCmd(0x80 + y * 0x40 + x);
	
	//2004
	if (row > LCD2440_MAX_ROW)
		row = 0;
	
	if (row == LCD2004_ROW0)
		LCDCmd(LCD2004_ROW0_ADDRESS_START + column);
	else if (row == LCD2004_ROW1)
		LCDCmd(LCD2004_ROW1_ADDRESS_START + column);
	else if (row == LCD2004_ROW2)
		LCDCmd(LCD2004_ROW2_ADDRESS_START + column);
	else if (row == LCD2004_ROW3)
		LCDCmd(LCD2004_ROW3_ADDRESS_START + column);
}

void LCDXYChar(uint8_t x, uint8_t y, const char c) // Display char at x:0-15, y:0-1
{
	LCDXY(x, y);
	LCDData(c);
}

void LCDXYStr(uint8_t x, uint8_t y, const char *str) // Display string at x:0-15, y:0-1
{
	const char *p;
	char c;

	LCDXY(x, y);
	p = str;
	while ((c = *p++) != 0)
		LCDData(c);
}

void LCDFullStr(const char *str) // Display string to full(2x16 chars) LCD, fill with blank if string is less than 32 chars
{
	const char *p;
	char c;
	uint8_t x, y;
	uint8_t bInStr;

	p = str;
	bInStr = 1;
	for (y = 0; y <= 1; y++)
	{
		for (x = 0; x <= 15; x++)
		{
			if (!x)
				LCDXY(0, y);

			if (bInStr)
			{
				c = *(p++);
				if (!c)
				{
					c = ' ';
					bInStr = 0;
				}
			}
			else
				c = ' ';

			LCDData(c);
		}
	}
} // void LCDFullStr_P(PGM_P str)  // Display PROGMEM string to full(2x16 chars) LCD, fill with blank if string is less than 32 chars

// Display string at x:0-15, y:0-1
void LCDXYStrLen(uint8_t x, uint8_t y, const char *str, uint8_t nLen, uint8_t bLeftAlign)
{
	char *p, c;
	uint8_t i, n;

	LCDXY(x, y);
	p = (char *)str;
	n = nLen - strlen(str);
	if (!bLeftAlign)
	{
		for (i = 0; i < n; i++)
			LCDData(' ');
	}

	while ((c = *p++) != 0)
		LCDData(c);

	if (bLeftAlign)
	{
		for (i = 0; i < n; i++)
			LCDData(' ');
	}
} // void LCDXYStrLen(uint8_t x, uint8_t y, char *str, uint8_t nLen, bool bLeftAlign)

// Display signed integer at x:0-15, y:0-1. Righ aligned, spaces filled at left. Too long integers are not allowed.
void LCDXYIntLen(uint8_t x, uint8_t y, int32_t n, uint8_t nLen)
{
	int8_t i;
	int32_t r;

	if (n >= 0)
		r = n;
	else
		r = -n;

	i = nLen - 1;
	do
	{
		LCDXYChar(x + i--, y, '0' + (r % 10));
		r = r / 10;
	} while (r);

	if (n < 0)
		LCDXYChar(x + i--, y, '-');

	while (i >= 0)
		LCDXYChar(x + i--, y, ' ');
}

// Display unsigned integer at x:0-15, y:0-1. Righ aligned, '0' filled at left. Too long unsigned integers are not allowed.
void LCDXYUIntLenZP(uint8_t x, uint8_t y, uint32_t n, uint8_t nLen)
{
	int8_t i;
	uint32_t r;
	r = n;
	i = nLen - 1;
	do
	{
		LCDXYChar(x + i--, y, '0' + (r % 10));
		r = r / 10;
	} while (r);

	while (i >= 0)
		LCDXYChar(x + i--, y, '0');
}
