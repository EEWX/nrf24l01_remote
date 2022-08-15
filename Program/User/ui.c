#include "stm8l15x.h"
#include "lcd19264.h"
#include "ui.h"
#include "encoder.h"

Menu_Typedef MenuList;
Window_Typedef UI_Table[5]=
{
  {0,"    WORKING     ","TIP:123CSET:    "},
  {1,"    SLEEPING    ","TIP:123CSET:    "},
  {2,"    SETTING1    ","WORKING SETPOINT"},
  {3,"    SETTING2    ","SLEEPING SETPOIN"},
  {4,"    SETTING3    ","TIP:123  SET:   "},
};

void UI_Init(void)
{
  MenuList.Page = 0;
  MenuList.PageMax = 4;
}

void UI_Display(Window_Typedef *win)
{
  LCD1602_DisplayString(0,0,win->Line_H);
  LCD1602_DisplayString(0,1,win->Line_L);
}

void UI_KeyProcess(Key_TypeDef *key)
{
  switch(*key)
  {
    case Key_Null:
      
      break;
    case Key_Push:
      
      break;
    case Key_Increase:
      MenuList.Page++;
      if(MenuList.Page > MenuList.PageMax)
      {
        MenuList.Page = 0;
      }
      break;
    case Key_Decrease:
      MenuList.Page--;
      if(MenuList.Page < 0)
      {
        MenuList.Page = MenuList.PageMax;
      }
      break;
    default:
      break;
  }
  *key = Key_Null;
}