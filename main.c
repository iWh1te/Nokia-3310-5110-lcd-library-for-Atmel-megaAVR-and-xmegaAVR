/*
 * Nokia3310_LCD_lib.c
 *
 * Created: 12.08.2016 22:57:56
 * Author : admin
 */ 

#define F_CPU	2000000ul


#include <avr/io.h>
#include <util/delay.h>
#include "n5110.h"

int main(void)
{
    /* Replace with your application code */
	GLCD_Init();

    while (1) 
    {
		
			for (int y = 0 ; y < RESOLUTION_Y; y++)
			{
				for (int x = 0; x < RESOLUTION_X; x++)
				{
					GLCD_DrawPixel(x, y, PIXEL_XOR);
					GLCD_UpdateAlign(x, x, y, y);
				}
			}
			for (int y = 0 ; y < RESOLUTION_Y; y++)
			{
				for (int x = 0; x < RESOLUTION_X; x++)
				{
					GLCD_DrawPixel(x, y, PIXEL_XOR);
					GLCD_UpdateAlign(x, x, y, y);
				}
			}
			_delay_ms(1000);
			
		GLCD_DrawLine(0, 0, 84, 48, PIXEL_ON);
		GLCD_DrawLine(84, 0, 0, 48, PIXEL_ON);
		GLCD_DrawLine(0, 24, 84, 24, PIXEL_ON);
		GLCD_DrawRectangle(0, 5, 10, 40, PIXEL_ON);	
		GLCD_DrawCircle(42, 24, 20, PIXEL_ON);
		
		GLCD_Update();
    }
}

