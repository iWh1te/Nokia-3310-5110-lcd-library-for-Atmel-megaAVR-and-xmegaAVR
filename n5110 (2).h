

/*
* Описание     :  Драйвер для графического LCD от Nokia 5110 для megaAVR и xmegaAVR
* Автор        :  iWh1te <vcl.wh1te@gmail.com>
* Веб-страница :
*
*/

//#define _BV(x) (1<<x)


/************************************************************************/
/*	Инклюдить шрифты в n5110.c												*/
/*	Так же добать в перечисление имя шрифта									*/
/*	Добавление функции вывода строки смотреть пример в конце файла n5110.h  */
/************************************************************************/


#ifndef _N5110_H_
#define _N5110_H_

#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdbool.h>

// закомментируйте эту директиву, если ваш дисплей оригинальный
//#define CHINA_LCD
// выберите используемы тип контроллера
 //#define MEGA_AVR
 #define XMEGA_AVR
 
 //#define USE_SOFTWARE_SPI	//для использования программного SPI расскоментировать. ВНИМАНИЕ ВОЗМОЖНА МЕДЛЕННАЯ СКОРОСТЬ ОБНОВЛЕНИЯ ЭКРАНА
 
// Порт к которому подключен LCD (здесь пример распиновки для ATmega8)
// Библиотека использует аппаратный SPI, поэтому если хотите иной порт - придется реализовать SPI программно)
#ifdef MEGA_AVR
	#define LCD_PORT                   PORTB
	#define LCD_DDR                    DDRB4
 
	// Распиновка порта
	#define LCD_DC_PIN                 (1<<PORTB1)//PB1 
	#define LCD_CE_PIN                 PB2
	#define SPI_MOSI_PIN               PB3   // SDIN дисплея обязательно подключаем к MOSI аппаратного SPI
	#define LCD_RST_PIN                PB4
	#define SPI_CLK_PIN                PB5   // SCLK дисплея обязательно подключаем к SCK аппаратного SPI
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

typedef unsigned char              byte;		//Определение byte

byte Px, Py; //Координаты X Y
#define RESOLUTION_X	84		//Разрешение по Х
#define RESOLUTION_Y	48		//Разрешение по Y

// Макрос для установки курсора в позицию X Y
#define GLCD_GoTo(x, y)		Px = x;\
							Py = y;

// Перечисления
typedef enum
{
    LCD_CMD  = 0,     // Команда
    LCD_DATA = 1      // Данные
} LcdCmdData;

typedef enum
{
    PIXEL_OFF =  0,   // Погасить пиксели дисплея
    PIXEL_ON  =  1,   // Включить пиксели дисплея
    PIXEL_XOR =  2    // Инвертировать пиксели
} LcdPixelMode;



//таблица шрифтов
typedef enum
{
    FONT_1X = 1,      // Обычный размер шрифта 5x7
    FONT_2X = 2       // Увеличенный размер шрифта
} LcdFontSize;



/**
		?\Функция инициализцации SPI
*/
void SPI_Init(void);

/**
		?\Функция отправки данных по SPI
		byte *Byte - указатель на отправляемые данные
*/
void SPI_WriteByte(byte *Byte);

/**
		?\Функция отправки данных по SPI
		byte Byte - данные для записи в LCD
		LcdCmdData CmdData - Данные/команда
*/
void GLCD_WriteByte(byte Byte, LcdCmdData CmdData);

/**
		?\Функция инициализцации дислпея
*/
void GLCD_Init(void); 

/**
		?\Функция установки контраста
		byte Value - начальная координата X
*/
void GLCD_SetContrast(byte Value);

/**
		?\Функция очистки буфера
*/
void GLCD_CLearBuffer(void);

/**
		?\Функция обновления экрана
*/
void GLCD_Update(void);

/**
		?\Функция области экрана
					byte Xi - начальная координата X
					byte Xj - конечная координата X
					byte Yi - начальная координата Y
					byte Yj - конечная координата Y		
*/
void GLCD_UpdateAlign(byte Xi, byte Xj, byte Yi, byte Yj);

/**
		?\Функция установки пикселя в выбранной позиции
		Принимает значения:
					byte x - координата X
					byte y - координата Y
					LcdPixelMode pixelMode - режим пикселя
*/
void GLCD_DrawPixel(byte x, byte y, LcdPixelMode pixelMode);

/**
		?\Функция установки пикселя в выбранной позиции
		Принимает значения:
					byte Xi - начальная координата X
					byte Xj - конечная координата X
					byte Yi - начальная координата Y
					byte Yj - конечная координата Y
*/
void GLCD_FillAlign(byte Xi, byte Xj, byte Yi, byte Yj, LcdPixelMode pixelMode);

/**
		?\Функция вывода строки
		Принимает значения:
					const char *string - строка для вывода
					LcdFontSize fontSize - тип шрифта
					bool inversion - признак инверсии
*/
void GLCD_DrawString(const char *string, LcdFontSize fontSize, bool inversion);

/**
		?\Функция вывода строки с float
		Принимает значения:
					const char *string - строка для вывода
					LcdFontSize fontSize - тип шрифта
					bool inversion - признак инверсии
*/
void GLCD_DrawStringFloat(void);

/**
		?\Функция вывода соимвола
		Принимает значения:
					const char *string - строка для вывода
					LcdFontSize fontSize - тип шрифта
					bool inversion - признак инверсии
*/
void GLCD_DrawChar(char charachter, LcdFontSize fontSize, bool inversion);

/**
		?\Функция поиска соимвола
		Принимает значения:
					char charachter - симовол
					LcdFontSize fontSize - тип шрифта
*/
char GLCD_SearchChar(char charachter, LcdFontSize fontSize);

/**
		?\Функция отрисовки линии
		Принимает значения:
					byte Xi - начальная координата по Х
					byte Yi - начальная коорлината по Y
					byte Xj - конечная координата по X
					byte Yj - конечная коорлината по Y
					LcdPixelMode - режим пикселя
*/  
void GLCD_DrawLine(byte Xi, byte Yi, byte Xj, byte Yj, LcdPixelMode pixelMode);

/**
		?\Функция отрисовки прямоугольника
		Принимает значения:
					byte baseX - коорлината левого верхнего угла по Х
					byte baseY - коорлината левого верхнего угла по Y
					byte Height - высота прямоугольника
					byte Width - ширина прямоугольника
					LcdPixelMode - режим пикселя
*/  
void GLCD_DrawRectangle(byte baseX, byte baseY, byte Height, byte Width, LcdPixelMode pixelMode);

/**
		?\Функция отрисовки круга
		Принимает значения:
					byte X - коорлината центра по Х
					byte Y - коорлината центра по Y
					byte radius - радиус
					LcdPixelMode - режим пикселя
*/  
void GLCD_DrawCircle(byte X, byte Y, byte radius, LcdPixelMode pixelMode);

#endif  /*  _N5110_H_ */

/*

			uint8_t width = pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch)); //следующая позиция
			do
			{
				if (Py % 8 == 0)
				{
					for (uint8_t i = 0; i < 2; i++)
					{
						Screen[Px+y][(Py/8 + i)] |= pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x + i);  //
					}
				}
				else //корректный вывод
				{
					Screen[Px+y][(Py/8)] |= pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x) << (Py%8);  //
					Screen[Px+y][(Py/8+1)] |= pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x) >> (8-(Py%8)) | pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x+1) << (Py%8);
					Screen[Px+y][(Py/8+2)] |= pgm_read_byte(Tahoma14x14 + ((TAHOMA14X14_X*2+1) * ch) + x+1) >> (8-(Py%8));
				}
				x = x + 2;
				y++;
				CHECK_PX(Px+y);
			}while (x < TAHOMA14X14_X*2+1);		//придумать как использовать ширину	символа динамически
			Px += width+1; //следующая позиция

*/