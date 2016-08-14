

/*
* Îïèñàíèå     :  Äðàéâåð äëÿ ãðàôè÷åñêîãî LCD îò Nokia 5110 äëÿ megaAVR è xmegaAVR
* Àâòîð        :  iWh1te <vcl.wh1te@gmail.com>
* Âåá-ñòðàíèöà :  
*
*/


#include "n5110.h"

//Ñþäà èíêëþäèòü øðèôòû
//Òàê æå äîáàâèòü â òàáëèöó øðèôòîâ LcdFontSize
#include "Default_font.h"






static byte Screen[RESOLUTION_X][RESOLUTION_Y/8];	//áóôåð ýêðàíà

#define CHECK_PX(x)				if(x > RESOLUTION_X) {Px = 0;}	//ïðîâåðêà íà ïðåâûøåíèå ãðàíèöû

/**
		?\Ôóíêöèÿ íå êàëèáðîâàííîé çàäåðæêè
*/
static void Delay(void);

static void Delay(void)
{
	int i;
	for ( i = -32000; i < 32000; i++ );
}


/************************************************************************/
/*                    Ðàáîòà ñ SPI				                        */
/************************************************************************/

void SPI_Init(void)
{
	#ifdef USE_SOFTWARE_SPI
		#ifdef MEGA_AVR
		#endif

		#ifdef XMEGA_AVR
		#endif		
	#else		
		#ifdef MEGA_AVR		//Äëÿ megaAVR
			SPCR = 0x50; // áåç ïðåðûâàíèé, ñòàðøèé áèò ïåðâûé, ðåæèì ìàñòåðà, CPOL->0, CPHA->0, Clk/4
		#endif
			
		#ifdef XMEGA_AVR	//Äëÿ xmegaAVR
			LCD_SPI_MODULE.CTRL = SPI_MASTER_bm | SPI_PRESCALER_DIV4_gc | SPI_ENABLE_bm | SPI_MODE_0_gc;
		#endif	
	#endif

}

void SPI_WriteByte(byte *Byte)
{
	#ifdef USE_SOFTWARE_SPI
		LCD_PORT |= _BV(SPI_CLK_PIN);
		for (uint8_t bit = 0; bit < 8; bit++)
		{
			if ((*Byte << bit) & 0x80)
			{
				LCD_PORT |= _BV(SPI_MOSI_PIN);
			}
			else
			{
				LCD_PORT &= ~_BV(SPI_MOSI_PIN);
			}
			LCD_PORT &= ~_BV(SPI_CLK_PIN);
			asm("nop");
			LCD_PORT |= _BV(SPI_CLK_PIN);
			LCD_PORT &= ~_BV(SPI_MOSI_PIN);
		}
	#else	
		#ifdef MEGA_AVR		//Äëÿ megaAVR
			SPDR = data;
			// Æäåì îêîí÷àíèÿ ïåðåäà÷è
			while ( (SPSR & 0x80) != 0x80 );
		#endif
	
		#ifdef XMEGA_AVR	//Äëÿ xmegaAVR
			LCD_SPI_MODULE.DATA = *Byte;
			while (!(LCD_SPI_MODULE.STATUS & SPI_IF_bm));
		#endif
	#endif
}

void GLCD_WriteByte(byte Byte, LcdCmdData CmdData)
{
	// Âêëþ÷àåì êîíòðîëëåð äèñïëåÿ (íèçêèé óðîâåíü àêòèâíûé)
	LCD_PORT &= ~( _BV( LCD_CE_PIN ) );

	if ( CmdData == LCD_DATA )
	{
		LCD_PORT |= _BV( LCD_DC_PIN );
	}
	else
	{
		LCD_PORT &= ~( _BV( LCD_DC_PIN ) );
	}

	SPI_WriteByte(&Byte);	// Îòïðàâêà äàííûõ â êîíòðîëëåð äèñïëåÿ
	
	// Îòêëþ÷àåì êîíòðîëëåð äèñïëåÿ
	LCD_PORT |= _BV( LCD_CE_PIN );
}

//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/*                    Ðàáîòà ñ ýêðàíîì è áóôåðîì                        */
/************************************************************************/
void GLCD_Init(void)
{
	// Pull-up íà âûâîä ïîäêëþ÷åííûé ê reset äèñïëåÿ
	LCD_PORT |= _BV ( LCD_RST_PIN );

	// Óñòàíàâëèâàåì íóæíûå áèòû ïîðòà íà âûõîä
	LCD_DDR |= _BV( LCD_RST_PIN ) | _BV( LCD_DC_PIN ) | _BV( LCD_CE_PIN ) | _BV( SPI_MOSI_PIN ) | _BV( SPI_CLK_PIN );

	Delay();

	// Äåðãàåì reset
	LCD_PORT &= ~( _BV( LCD_RST_PIN ) );
	Delay();
	LCD_PORT |= _BV ( LCD_RST_PIN );

	SPI_Init();	// Àêòèâèðóåì SPI:

	// Îòêëþ÷àåì LCD êîíòðîëëåð - âûñîêèé óðîâåíü íà SCE
	LCD_PORT |= _BV( LCD_CE_PIN );

	// Îòïðàâëÿåì êîìàíäû äèñïëåþ
	GLCD_WriteByte(0x21, LCD_CMD);	// Âêëþ÷àåì ðàñøèðåííûé íàáîð êîìàíä (LCD Extended Commands)
	GLCD_WriteByte(0xC8, LCD_CMD);	// Óñòàíîâêà êîíòðàñòíîñòè (LCD Vop)
	GLCD_WriteByte(0x06, LCD_CMD);	// Óñòàíîâêà òåìïåðàòóðíîãî êîýôôèöèåíòà (Temp coefficent)
	GLCD_WriteByte(0x13, LCD_CMD);	// Íàñòðîéêà ïèòàíèÿ (LCD bias mode 1:48)
	GLCD_WriteByte(0x20, LCD_CMD);	// Âêëþ÷àåì ñòàíäàðòíûé íàáîð êîìàíä è ãîðèçîíòàëüíóþ àäðåñàöèþ (LCD Standard Commands,Horizontal addressing mode)
	GLCD_WriteByte(0x0C, LCD_CMD);	// Íîðìàëüíûé ðåæèì (LCD in normal mode)
	
	GLCD_CLearBuffer();
	GLCD_Update();
}

void GLCD_SetContrast(byte Value)
{
	GLCD_WriteByte( 0x21, LCD_CMD );              // Ðàñøèðåííûé íàáîð êîìàíä
	GLCD_WriteByte( 0x80 | Value, LCD_CMD );	  // Óñòàíîâêà óðîâíÿ êîíòðàñòíîñòè
	GLCD_WriteByte( 0x20, LCD_CMD );              // Ñòàíäàðòíûé íàáîð êîìàíä, ãîðèçîíòàëüíàÿ àäðåñàöèÿ
}

void GLCD_CLearBuffer(void)
{
	for (byte y = 0; y <RESOLUTION_Y / 8; y++)
	{
		for (byte x = 0; x < RESOLUTION_X; x++)
		{
			Screen[x][y] = 0x00;
		}
	}
}

void GLCD_Update(void)
{
	#ifdef CHINA_LCD		//Åñëè èñïîëüçóåòñÿ êèòàéñêèé ýêðàê
		for (byte y = 0; y < RESOLUTION_Y / 8; y++)
		{
			GLCD_WriteByte(0x80, LCD_CMD); //óñòàíîâêà êîîðäèíàò ïî îñè X
			GLCD_WriteByte(0x40 | y, LCD_CMD); 
			for (byte x = 0; x < RESOLUTION_X; x++)
			{
				GLCD_WriteByte(Screen[x][y], LCD_DATA);
			}
		}
		GLCD_WriteByte(0x21, LCD_CMD);
		GLCD_WriteByte(0x45, LCD_CMD);
		GLCD_WriteByte(0x20, LCD_CMD);
	#else			//Åñëè èñïîëüçóåòñÿ îðèãèíàëüíûé
		for (byte y = 0; y < RESOLUTION_Y / 8; y++)
		{
			GLCD_WriteByte(0x80, LCD_CMD); //óñòàíîâêà êîîðäèíàò ïî îñè X
			GLCD_WriteByte(0x40 | y, LCD_CMD);
			for (byte x = 0; x < RESOLUTION_X; x++)
			{
				GLCD_WriteByte(Screen[x][y], LCD_DATA);
			}
		}
		GLCD_WriteByte(0x21, LCD_CMD);
		GLCD_WriteByte(0x20, LCD_CMD);	
	#endif
}

void GLCD_UpdateAlign(byte Xi, byte Xj, byte Yi, byte Yj)
{
	#ifdef CHINA_LCD		//Åñëè èñïîëüçóåòñÿ êèòàéñêèé ýêðàê
	
		for (byte y = Yi; y <= Yj; y++)
		{
			GLCD_WriteByte(0x80 | Xi, LCD_CMD); //óñòàíîâêà êîîðäèíàò ïî îñè X
			GLCD_WriteByte(0x40 | y/8, LCD_CMD);
			for (byte x = Xi; x <= Xj; x++)
			{
				GLCD_WriteByte(Screen[x][y/8], LCD_DATA);
			}
		}
		GLCD_WriteByte(0x21, LCD_CMD);
		GLCD_WriteByte(0x45, LCD_CMD);
		GLCD_WriteByte(0x20, LCD_CMD);
	
	#else			//Åñëè èñïîëüçóåòñÿ îðèãèíàëüíûé
	
		for (byte y = Yi; y <= Yj; y++)
		{
			GLCD_WriteByte(0x80 | Xi, LCD_CMD); //óñòàíîâêà êîîðäèíàò ïî îñè X
			GLCD_WriteByte(0x40 | y/8, LCD_CMD);
			for (byte x = Xi; x <= Xj; x++)
			{
				GLCD_WriteByte(Screen[x][y/8], LCD_DATA);
			}
		}
		GLCD_WriteByte(0x21, LCD_CMD);
		GLCD_WriteByte(0x20, LCD_CMD);		
	#endif	
}

void GLCD_DrawPixel(byte x, byte y, LcdPixelMode pixelMode)
{
	switch(pixelMode)
	{
		case PIXEL_ON:
		{
			Screen[x][y/8] |= (0x01 << (y%8));
			break;
		}
		case PIXEL_OFF:
		{
			Screen[x][y/8] &= ( 0xff ^ (0x01 << ((y%8))));
			break;
		}
		case PIXEL_XOR:
		{
			Screen[x][y/8] ^= (0x01 << (y%8));
			break;
		}
	}
}

void GLCD_FillAlign(byte Xi, byte Xj, byte Yi, byte Yj, LcdPixelMode pixelMode)
{
	for (byte y = Yi; y < Yj; y++)
	{
		for (byte x = Xi; x < Xj; x++)
		{
			GLCD_DrawPixel(x, y, pixelMode);
		}
	}
}

//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/*                    Ðàáîòà ñî ñòðîêàìè.                               */
/************************************************************************/
void GLCD_DrawChar(char charachter, LcdFontSize fontSize, bool inversion)
{	
	charachter = GLCD_SearchChar(charachter, fontSize);
	
	switch(fontSize)
	{
		case FONT_1X:
		{
			if (inversion)
			{
				GLCD_FillAlign(Px-1, Px+FONT_1X_X+1, Py-1, Py+FONT_1X_Y+1, PIXEL_ON);	//çàëèâàåì ïîëèãîí äëÿ ôîíà
				for (byte x = 0; x < FONT_1X_X; x++)
				{
					if (Py%8 == 0)
					{
						Screen[Px+x][(Py/8)] ^= pgm_read_byte(font5x7 + (FONT_1X_X * charachter) + x);
						CHECK_PX(Px+x);
					}
					else														//îòðèñîâêà ñèìâîëà
					{
						Screen[Px+x][(Py/8)] ^= pgm_read_byte(font5x7 + (FONT_1X_X * charachter) + x)  << (Py%8);  //
						Screen[Px+x][(Py/8+1)] ^= pgm_read_byte(font5x7 + (FONT_1X_X * charachter) + x)  >> (8-(Py%8));
					}
					CHECK_PX(Px+x);
				}
			}
			else
			{
				for (byte x = 0; x < FONT_1X_X; x++)
				{
					if (Py % 8 == 0)
					{
						Screen[Px+x][(Py/8)] |= pgm_read_byte(font5x7 + (FONT_1X_X * charachter) + x);
					}
					else
					{
						Screen[Px+x][(Py/8)] |= pgm_read_byte(font5x7 + (FONT_1X_X * charachter) + x)  << (Py%8);  //
						Screen[Px+x][(Py/8+1)] |= pgm_read_byte(font5x7 + (FONT_1X_X * charachter) + x)  >> (8-(Py%8));
					}
					CHECK_PX(Px+x);
				}
			}
			Px += FONT_1X_X+1; //ñëåäóþùàÿ ïîçèöèÿ
			break;
		}//end case FONT_1x
		case FONT_2X:
		{
			break;
		}
	}
}

void GLCD_DrawString(const char *string, LcdFontSize fontSize, bool inversion)
{
	while(*string)
	{
		GLCD_DrawChar(*string++, fontSize, inversion);
	}
}

void GLCD_DrawStringFloat(void)
{
	
}

char GLCD_SearchChar(char charachter, LcdFontSize fontSize)
{
	switch(fontSize)
	{
		case FONT_1X:
		{
			if ((charachter >= 0x20) && (charachter <= 0x7E))
			{
				charachter -= 0x20;
			}
			else if ((charachter >= 0xC0) && (charachter <= 0xFF))
			{
				charachter -= 0x5F;
			}
			break;
		}
		case FONT_2X:
		{
			break;
		}
	}
	return charachter;
}

//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/*                    Ðàáîòà c ãðàôèêîé.                               */
/************************************************************************/

void GLCD_DrawLine(byte Xi, byte Yi, byte Xj, byte Yj, LcdPixelMode pixelMode)
{
    const int deltaX = abs(Xj - Xi);
    const int deltaY = abs(Yj - Yi);
    const int signX = Xi < Xj ? 1 : -1;
    const int signY = Yi < Yj ? 1 : -1;
    //
    int error = deltaX - deltaY;
    //
    GLCD_DrawPixel(Xj, Yj, pixelMode);
    while(Xi != Xj || Yi != Yj)
    {
	    GLCD_DrawPixel(Xi, Yi, pixelMode);
	    const int error2 = error * 2;
	    //
	    if(error2 > -deltaY)
	    {
		    error -= deltaY;
		    Xi += signX;
	    }
	    if(error2 < deltaX)
	    {
		    error += deltaX;
		    Yi += signY;
	    }
    }
}

void GLCD_DrawRectangle(byte baseX, byte baseY, byte Height, byte Width, LcdPixelMode pixelMode)
{	
	//ðèñóåì ãîðèçîíòàëüíûå ëèíèè
	GLCD_DrawLine(baseX, baseY, baseX + Width, baseY, pixelMode);
	GLCD_DrawLine(baseX, baseY + Height, baseX + Width, baseY + Height, pixelMode);
	//ðèñóåì âåðòèêàëüíûå ëèíèè
	GLCD_DrawLine(baseX, baseY, baseX, baseY + Height, pixelMode);
	GLCD_DrawLine(baseX + Width, baseY, baseX + Width, baseY + Height, pixelMode);
}

void GLCD_DrawCircle(byte X, byte Y, byte radius, LcdPixelMode pixelMode)
{
	signed char xc = 0;
	signed char yc = 0;
	signed char p = 0;

	yc = radius;
	p = 3 - (radius<<1);
	while (xc <= yc)
	{
		GLCD_DrawPixel(X + xc, Y + yc, pixelMode);
		GLCD_DrawPixel(X + xc, Y - yc, pixelMode);
		GLCD_DrawPixel(X - xc, Y + yc, pixelMode);
		GLCD_DrawPixel(X - xc, Y - yc, pixelMode);
		GLCD_DrawPixel(X + yc, Y + xc, pixelMode);
		GLCD_DrawPixel(X + yc, Y - xc, pixelMode);
		GLCD_DrawPixel(X - yc, Y + xc, pixelMode);
		GLCD_DrawPixel(X - yc, Y - xc, pixelMode);
		if (p < 0) p += (xc++ << 2) + 6;
		else p += ((xc++ - yc--)<<2) + 10;
	}

	// Óñòàíîâêà ôëàãà èçìåíåíèé êýøà
}

//////////////////////////////////////////////////////////////////////////

