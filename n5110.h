

/*
* ��������     :  ������� ��� ������������ LCD �� Nokia 5110 ��� megaAVR � xmegaAVR
* �����        :  iWh1te <vcl.wh1te@gmail.com>
* ���-�������� :
*
*/

//#define _BV(x) (1<<x)


/************************************************************************/
/*	��������� ������ � n5110.c												*/
/*	��� �� ������ � ������������ ��� ������									*/
/*	���������� ������� ������ ������ �������� ������ � ����� ����� n5110.h  */
/************************************************************************/


#ifndef _N5110_H_
#define _N5110_H_

#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdbool.h>

// ��������������� ��� ���������, ���� ��� ������� ������������
//#define CHINA_LCD
// �������� ����������� ��� �����������
 //#define MEGA_AVR
 #define XMEGA_AVR
 
 //#define USE_SOFTWARE_SPI	//��� ������������� ������������ SPI �����������������. �������� �������� ��������� �������� ���������� ������
 
// ���� � �������� ��������� LCD (����� ������ ���������� ��� ATmega8)
// ���������� ���������� ���������� SPI, ������� ���� ������ ���� ���� - �������� ����������� SPI ����������)
#ifdef MEGA_AVR
	#define LCD_PORT                   PORTB
	#define LCD_DDR                    DDRB4
 
	// ���������� �����
	#define LCD_DC_PIN                 (1<<PORTB1)//PB1 
	#define LCD_CE_PIN                 PB2
	#define SPI_MOSI_PIN               PB3   // SDIN ������� ����������� ���������� � MOSI ����������� SPI
	#define LCD_RST_PIN                PB4
	#define SPI_CLK_PIN                PB5   // SCLK ������� ����������� ���������� � SCK ����������� SPI
#endif

#ifdef	XMEGA_AVR 
	#define LCD_SPI_MODULE				SPIC
	#define LCD_PORT					PORTC.OUT
	#define LCD_DDR						PORTC.DIR
	
	#define LCD_DC_PIN					3
	#define LCD_CE_PIN					4
	#define SPI_MOSI_PIN				5
	#define LCD_RST_PIN					6
	#define SPI_CLK_PIN					7
 #endif

typedef unsigned char              byte;		//����������� byte

byte Px, Py; //���������� X Y
#define RESOLUTION_X	84		//���������� �� �
#define RESOLUTION_Y	48		//���������� �� Y

// ������ ��� ��������� ������� � ������� X Y
#define GLCD_GoTo(x, y)		Px = x;\
							Py = y;

// ������������
typedef enum
{
    LCD_CMD  = 0,     // �������
    LCD_DATA = 1      // ������
} LcdCmdData;

typedef enum
{
    PIXEL_OFF =  0,   // �������� ������� �������
    PIXEL_ON  =  1,   // �������� ������� �������
    PIXEL_XOR =  2    // ������������� �������
} LcdPixelMode;



//������� �������
typedef enum
{
    FONT_1X = 1,      // ������� ������ ������ 5x7
    FONT_2X = 2       // ����������� ������ ������
} LcdFontSize;



/**
		?\������� �������������� SPI
*/
void SPI_Init(void);

/**
		?\������� �������� ������ �� SPI
		byte *Byte - ��������� �� ������������ ������
*/
void SPI_WriteByte(byte *Byte);

/**
		?\������� �������� ������ �� SPI
		byte Byte - ������ ��� ������ � LCD
		LcdCmdData CmdData - ������/�������
*/
void GLCD_WriteByte(byte Byte, LcdCmdData CmdData);

/**
		?\������� �������������� �������
*/
void GLCD_Init(void); 

/**
		?\������� ��������� ���������
		byte Value - ��������� ���������� X
*/
void GLCD_SetContrast(byte Value);

/**
		?\������� ������� ������
*/
void GLCD_CLearBuffer(void);

/**
		?\������� ���������� ������
*/
void GLCD_Update(void);

/**
		?\������� ������� ������
					byte Xi - ��������� ���������� X
					byte Xj - �������� ���������� X
					byte Yi - ��������� ���������� Y
					byte Yj - �������� ���������� Y		
*/
void GLCD_UpdateAlign(byte Xi, byte Xj, byte Yi, byte Yj);

/**
		?\������� ��������� ������� � ��������� �������
		��������� ��������:
					byte x - ���������� X
					byte y - ���������� Y
					LcdPixelMode pixelMode - ����� �������
*/
void GLCD_DrawPixel(byte x, byte y, LcdPixelMode pixelMode);

/**
		?\������� ��������� ������� � ��������� �������
		��������� ��������:
					byte Xi - ��������� ���������� X
					byte Xj - �������� ���������� X
					byte Yi - ��������� ���������� Y
					byte Yj - �������� ���������� Y
*/
void GLCD_FillAlign(byte Xi, byte Xj, byte Yi, byte Yj, LcdPixelMode pixelMode);

/**
		?\������� ������ ������
		��������� ��������:
					const char *string - ������ ��� ������
					LcdFontSize fontSize - ��� ������
					bool inversion - ������� ��������
*/
void GLCD_DrawString(const char *string, LcdFontSize fontSize, bool inversion);

/**
		?\������� ������ ������ � float
		��������� ��������:
					const char *string - ������ ��� ������
					LcdFontSize fontSize - ��� ������
					bool inversion - ������� ��������
*/
void GLCD_DrawStringFloat(void);

/**
		?\������� ������ ��������
		��������� ��������:
					const char *string - ������ ��� ������
					LcdFontSize fontSize - ��� ������
					bool inversion - ������� ��������
*/
void GLCD_DrawChar(char charachter, LcdFontSize fontSize, bool inversion);

/**
		?\������� ������ ��������
		��������� ��������:
					char charachter - �������
					LcdFontSize fontSize - ��� ������
*/
char GLCD_SearchChar(char charachter, LcdFontSize fontSize);

/**
		?\������� ��������� �����
		��������� ��������:
					byte Xi - ��������� ���������� �� �
					byte Yi - ��������� ���������� �� Y
					byte Xj - �������� ���������� �� X
					byte Yj - �������� ���������� �� Y
					LcdPixelMode - ����� �������
*/  
void GLCD_DrawLine(byte Xi, byte Yi, byte Xj, byte Yj, LcdPixelMode pixelMode);

/**
		?\������� ��������� ��������������
		��������� ��������:
					byte baseX - ���������� ������ �������� ���� �� �
					byte baseY - ���������� ������ �������� ���� �� Y
					byte Height - ������ ��������������
					byte Width - ������ ��������������
					LcdPixelMode - ����� �������
*/  
void GLCD_DrawRectangle(byte baseX, byte baseY, byte Height, byte Width, LcdPixelMode pixelMode);

/**
		?\������� ��������� �����
		��������� ��������:
					byte X - ���������� ������ �� �
					byte Y - ���������� ������ �� Y
					byte radius - ������
					LcdPixelMode - ����� �������
*/  
void GLCD_DrawCircle(byte X, byte Y, byte radius, LcdPixelMode pixelMode);

#endif  /*  _N5110_H_ */

/*

			uint8_t width = pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch)); //��������� �������
			do
			{
				if (Py % 8 == 0)
				{
					for (uint8_t i = 0; i < 2; i++)
					{
						Screen[Px+y][(Py/8 + i)] |= pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x + i);  //
					}
				}
				else //���������� �����
				{
					Screen[Px+y][(Py/8)] |= pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x) << (Py%8);  //
					Screen[Px+y][(Py/8+1)] |= pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x) >> (8-(Py%8)) | pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x+1) << (Py%8);
					Screen[Px+y][(Py/8+2)] |= pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x+1) >> (8-(Py%8));
				}
				x = x + 2;
				y++;
				CHECK_PX(Px+y);
			}while (x < TAHOMA14X14_X*2+1);		//��������� ��� ������������ ������	������� �����������
			Px += width+1; //��������� �������

*/