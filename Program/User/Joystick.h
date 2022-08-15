#ifndef JOYSTICK_H
#define JOYSTICK_H

#define CHANNEL_NUMBER 8
#define MAXIMUM_CNT 4000

#define ZERO_POSITION_DEBOUNCE 2

//------test center data --> 1924-2189
#define CENTER_POSITION_DEBOUNCE 155

#define CHANNEL_NEGATIVE_SIGN 0x8000
#define AUTO_ZERO_TYPE 0x4000

typedef enum
{
  AutoCenterType = (uint8_t)0x00,
  NoneAutoCenterType = (uint8_t)0x01,
}
Joystick_Typedef;

typedef struct ChannelStruct
{
  Joystick_Typedef JoystickType;
  FlagStatus ChannelReverseFlag;
  int ChannelOffset;
  uint16_t ChannelData;
}
Channel_Typedef;

extern Channel_Typedef JoystickChannel[8];
void JoystickInit(void);
uint16_t JOYSTICK_Update(uint16_t *p,Joystick_Typedef ctype,FlagStatus reverse);

#endif