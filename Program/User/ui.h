#ifndef UI_H
#define UI_H

#include "encoder.h"

typedef struct WindowStruct
{
  uint8_t WIndow_ID;
  char Line_H[17];
  char Line_L[17];
}
Window_Typedef;

typedef struct MenuStruct
{
  FlagStatus ItemChangeFlag;
  char Page;                    //current page number
  uint8_t PageMax;              //max page number
  uint8_t CursorX;
  uint8_t CursorY;
}
Menu_Typedef;

extern Window_Typedef UI_Table[5];
extern Menu_Typedef MenuList;

void UI_Init(void);
void UI_Display(Window_Typedef *win);
void UI_KeyProcess(Key_TypeDef *key);

#endif
