#include "stm8l15x.h"
#include "joystick.h"


Channel_Typedef JoystickChannel[8];

void JoystickInit(void)
{
  
  JoystickChannel[0].JoystickType = NoneAutoCenterType;
  JoystickChannel[1].JoystickType = AutoCenterType;
  JoystickChannel[2].JoystickType = AutoCenterType;
  JoystickChannel[3].JoystickType = AutoCenterType;
  JoystickChannel[4].JoystickType = AutoCenterType;
  JoystickChannel[5].JoystickType = AutoCenterType;
  JoystickChannel[6].JoystickType = AutoCenterType;
  JoystickChannel[7].JoystickType = AutoCenterType;
}
//-----------convert 0-4096 ADC data to 0-8000 
uint16_t JOYSTICK_Update(uint16_t *p,Joystick_Typedef ctype,FlagStatus reverse)
{
  uint16_t temp;
  switch(ctype)
  {
  case AutoCenterType:
    if((*p < 2048+CENTER_POSITION_DEBOUNCE) && (*p > 2048-CENTER_POSITION_DEBOUNCE))
    {
      temp = 0;
      temp &= 0x7fff; //positive value flag
    }
    else
    {
      if(*p > 2048+CENTER_POSITION_DEBOUNCE)
      {
         temp= *p - 2048 - CENTER_POSITION_DEBOUNCE;
         temp &= 0x7fff; //positive value flag
      }
      else
      {
        temp = 2048-CENTER_POSITION_DEBOUNCE-*p;
        temp |= CHANNEL_NEGATIVE_SIGN; //negative value flag
      }
      
    }
    temp |= AUTO_ZERO_TYPE;
    
    break;
  case NoneAutoCenterType:
    if(*p < ZERO_POSITION_DEBOUNCE)
    {
      temp = 0;
    }
    else
    {
      temp = *p - ZERO_POSITION_DEBOUNCE;
      temp &= 0x7fff; //positive value flag
    }
    temp &= ~AUTO_ZERO_TYPE;
    break;
  default:
    break;
  }
  
  if(reverse)
  {
    if(temp & CHANNEL_NEGATIVE_SIGN)
    {
      temp &= 0x7fff;
    }
    else
    {
      temp |= CHANNEL_NEGATIVE_SIGN;
    }
  }
  return(temp);
}