#ifndef __OLED_H
#define __OLED_H	
#include "stm32f10x.h"                  // Device header
#include "SYSTEM.h"


#define Max_Column	128
#define Max_Row		64 
#define OLED_DATA 1	//Ð´Êý¾Ý
#define OLED_MODE 0

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowChar1(u8 x, u8 y, u8 chr);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowNum2(u8 x, u8 y, u32 num, u8 len, u8 mode);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
#endif  

