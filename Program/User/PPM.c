#include "stm8l15x.h"
#include "ppm.h"

#define pulse_0p3_ms 1200
#define pulse_0p4_ms 1600
#define pulse_2_ms 8000
//---------------
uint16_t Channel_data[9] = {0};//start pulse(0.4ms) 8 channel 

void PPM_Init(void)
{
  GPIO_Init(PPM_GPIO,PPM_PIN,GPIO_Mode_Out_PP_High_Slow);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3,ENABLE);
  TIM3_DeInit();
  TIM3_TimeBaseInit(TIM3_Prescaler_4,TIM3_CounterMode_Up,16000); //250hz, low 2ms,high 2ms
  TIM3_OC2Init(TIM3_OCMode_PWM1,TIM3_OutputState_Enable,8000,TIM3_OCPolarity_High,TIM3_OCIdleState_Reset);
  TIM3_CtrlPWMOutputs(ENABLE);
  TIM3_CCxCmd(TIM3_Channel_2,ENABLE);
  TIM3_SelectOnePulseMode(TIM3_OPMode_Single);
  TIM3_Cmd(ENABLE);
  
  Channel_data[0] = pulse_0p4_ms; //0.4ms start pulse
  Channel_data[0] = 1200; //0.3ms start pulse
  
}

void PPM_Update(uint16_t *ptr)
{
  
}

