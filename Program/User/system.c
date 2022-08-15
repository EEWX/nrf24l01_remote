#include "stm8l15x.h"
#include "system.h"
#include "atomtimer.h"
#include "encoder.h"
#include <stdio.h>


void SYSTEM_AnalogCompensate(void);

void SYSTEM_Init(void)
{
  GPIO_Init(SYSTEM_PWR_HOLD_GPIO,SYSTEM_PWR_HOLD_PIN,GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(SYSTEM_PWR_SAVE_GPIO,SYSTEM_PWR_SAVE_PIN,GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(SYSTEM_PWR_KEY_GPIO,SYSTEM_PWR_KEY_PIN,GPIO_Mode_In_FL_No_IT);
  
  //---------Use DAC to generate DC offset to compensate thermal couple amplifier
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC,ENABLE);
  GPIO_Init(SYSTEM_DAC_GPIO,SYSTEM_DAC_PIN,GPIO_Mode_In_FL_No_IT);
  DAC_Init(DAC_Channel_1,DAC_Trigger_None,DAC_OutputBuffer_Enable);
  DAC_Cmd(DAC_Channel_1,ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_R,DEFAULT_DAC_VALUE);
  
//  SYSTEM_AnalogCompensate();
  
  //-----------Cross zero comparator configuration------------------
  CLK_PeripheralClockConfig(CLK_Peripheral_COMP,ENABLE);
  COMP_VrefintOutputCmd(ENABLE);
 // GPIO_Init(SYSTEM_VREF_GPIO,SYSTEM_VREF_PIN,GPIO_Mode_In_FL_No_IT);
  COMP_Init(COMP_InvertingInput_VREFINT,COMP_OutputSelect_TIM2IC2,COMP_Speed_Fast);
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_22,ENABLE);       //PD1 analog switch close
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_8,ENABLE);       //PD7 analog switch close, enable Vref output to PD7
  atomTimerDelay(10);
  
}


void SYSTEM_InternalReferenceOutput(FunctionalState NewState)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_COMP,ENABLE);
  COMP_VrefintOutputCmd(ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  ADC_VrefintCmd(ENABLE);
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_8,NewState);       //PD7 analog switch close, enable Vref output to PD7
}



