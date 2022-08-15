#ifndef LCD19264_H
#define LCD19264_H
#include "stm8l15x.h"

#define LCD19264_SCK_GPIO GPIOE //data clock
#define LCD19264_SCK_PIN GPIO_Pin_4

#define LCD19264_SDA_GPIO GPIOE  //data
#define LCD19264_SDA_PIN GPIO_Pin_3

#define LCD19264_RST_GPIO GPIOE
#define LCD19264_RST_PIN GPIO_Pin_2

#define LCD19264_CD_GPIO GPIOE  //command data
#define LCD19264_CD_PIN GPIO_Pin_1

#define LCD19264_CS_GPIO GPIOE
#define LCD19264_CS_PIN GPIO_Pin_0

#define LCD19264_LED_GPIO GPIOB
#define LCD19264_LED_PIN GPIO_Pin_0

#define LCD_SCK_SET	GPIO_SetBits(LCD19264_SCK_GPIO,LCD19264_SCK_PIN)
#define LCD_SDI_SET	GPIO_SetBits(LCD19264_SDA_GPIO,LCD19264_SDA_PIN)
#define LCD_RST_SET	GPIO_SetBits(LCD19264_RST_GPIO,LCD19264_RST_PIN)
#define LCD_A0_SET	GPIO_SetBits(LCD19264_CD_GPIO,LCD19264_CD_PIN)
#define LCD_CS_SET	GPIO_SetBits(LCD19264_CS_GPIO,LCD19264_CS_PIN)

#define LCD_SCK_RESET	GPIO_ResetBits(LCD19264_SCK_GPIO,LCD19264_SCK_PIN)
#define LCD_SDI_RESET	GPIO_ResetBits(LCD19264_SDA_GPIO,LCD19264_SDA_PIN)
#define LCD_RST_RESET	GPIO_ResetBits(LCD19264_RST_GPIO,LCD19264_RST_PIN)
#define LCD_A0_RESET	GPIO_ResetBits(LCD19264_CD_GPIO,LCD19264_CD_PIN)
#define LCD_CS_RESET	GPIO_ResetBits(LCD19264_CS_GPIO,LCD19264_CS_PIN)



#define LCD1602_PARALLEL_CMD_MODE 0x38              //16X2 display, each 5X7 dots, 8 bits parallel port

#define LCD1602_PARALLEL_REG_FUNCTION 0x08
#define LCD1602_PARALLEL_REG_FUNCTION_BLINK 0x01            //cursor blink switch
#define LCD1602_PARALLEL_REG_FUNCTION_CURSOR 0x02            //cursor display switch
#define LCD1602_PARALLEL_REG_FUNCTION_DISPLAY 0x04            //global display switch

#define LCD1602_PARALLEL_REG_ADDRESS_MODE 0x04
#define LCD1602_PARALLEL_REG_ADDRESS_MODE_SHIFT 0x01            //background shift, cursor stay.
#define LCD1602_PARALLEL_REG_ADDRESS_MODE_CURSOR 0x02            //cursor plus or minus control, 1: cursor increase after each write

#define LCD1602_PARALLEL_REG_DATA_ADDRESS 0x80                 //set data pointer, 0x80+address, 0-27H,40-67H

#define LCD1602_PARALLEL_CMD_CLEARSCREEN 0x01
#define LCD1602_PARALLEL_CMD_PTR_RETURN 0x02                    //data pointer reset to zero

#define LCD_LED_OFF()	GPIO_ResetBits(LCD19264_LED_GPIO,LCD19264_LED_PIN)

#define ICONs_PAGE_ADDRESS 8
#define ICON_ANTTENA 0
#define ICON_BATTERY_EMPTY 116
#define ICON_BATTERY_VERY_LOW 125
#define ICON_BATTERY_LOW 127
#define ICON_BATTERY_MEDIUM 123
#define ICON_BATTERY_FULL 120

typedef enum
{
	IconSignal = 0,
	IconCall = 34,
	IconLock = 47,
	IconMail = 60,
	IconRing = 75,
	IconClock = 88,
	IconVibrate = 103,
	IconBattery = 121,
}LCD_Icon_TypeDef;

void LCD19264_Init(void);
void LCD_ClearScreen(unsigned char dat);
void LCD_DisplayByte(uint8_t x, uint8_t y, uint8_t data);
void LCD_Put6x8Char(uint8_t x, uint8_t y, char asc);
void LCD_Put6x8IntNumber(uint8_t x, uint8_t y, uint16_t num);
void LCD_Put8x16Str(uint8_t x, uint8_t y, char *ptr);
void LCD_Put8x16IntNumber(uint8_t x, uint8_t y, uint16_t num);
void LCD_Draw8BitBar(uint8_t x1, uint8_t x2, uint8_t y, uint8_t cursor);
void LCD_LightIconSignal(uint8_t level,uint8_t status);
void LCD_LightIconBattery(uint8_t level,uint8_t status);

void LCD_RAM_Refresh(void);
void LCD_RAM_WriteDot(uint8_t x, uint8_t y, uint8_t dot);
void LCD_RAM_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void LCD_RAM_DrawCircle(uint8_t x1, uint8_t y1, uint8_t rad);
void LCD_RAM_Put6x8Char(uint8_t x, uint8_t y, char asc);
void LCD_RAM_Put6x8Str(uint8_t x, uint8_t y, char *ptr);
void LCD_RAM_Put8x16Char(uint8_t x, uint8_t y, char asc);
void LCD_RAM_Put8x16Str(uint8_t x, uint8_t y, char *ptr);
void LCD_Test(void);
#endif
