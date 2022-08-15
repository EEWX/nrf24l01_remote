#include "stm8l15x.h"
#include "encoder.h"
#include "handle.h"
#include "atomtimer.h"
#include "system.h"

Key_TypeDef Encoderkey;
FlagStatus KeyStatus = RESET;

void ENCODER_Init(void)
{
  GPIO_Init(ENCODER_A_GPIO,ENCODER_A_PIN,GPIO_Mode_In_PU_IT);
  GPIO_Init(ENCODER_B_GPIO,ENCODER_B_PIN,GPIO_Mode_In_PU_No_IT);
  GPIO_Init(ENCODER_BUTTON_GPIO,ENCODER_BUTTON_PIN,GPIO_Mode_In_PU_IT);
  GPIO_Init(ENCODER_BEEPER_GPIO,ENCODER_BEEPER_PIN,GPIO_Mode_Out_PP_Low_Slow);
  
 // EXTI_SetPinSensitivity(EXTI_Pin_6,EXTI_Trigger_Falling);      //encoder A input
  //EXTI_SetPinSensitivity(EXTI_Pin_7,EXTI_Trigger_Falling);      //encoder button input
  
}

void ENCODER_BeeperBeep(uint8_t time10ms)
{
  //TIM2_CtrlPWMOutputs(ENABLE);
  atomTimerDelay(time10ms);
  //TIM2_CtrlPWMOutputs(DISABLE);
}

void ENCODER_BeeperClickSound(void)
{
  GPIO_WriteBit(ENCODER_BEEPER_GPIO,ENCODER_BEEPER_PIN,SET);
  atomTimerDelay(1);
  GPIO_WriteBit(ENCODER_BEEPER_GPIO,ENCODER_BEEPER_PIN,RESET);
}

void ENCODER_Push_ISR(void)
{
 
}

void ENCODER_Rotate_ISR(void)
{
  
  if(GPIO_ReadInputDataBit(ENCODER_B_GPIO,ENCODER_B_PIN) != RESET)
  {
  }
  else
  {
    
  }
}



