#include "stm8l15x.h"
#include "adc.h"
uint16_t ADC_Results[ADC_BUFFER_SIZE];
uint16_t ADC_ResultsRaw[ADC_BUFFER_SIZE];
FlagStatus SYSTEM_ADC_UpdateFlag = RESET;

void Analog_Init(void)
{
    //-----------Cross zero comparator configuration------------------
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1,ENABLE);
  ADC_VrefintCmd(ENABLE);
  
  GPIO_Init(LEFT_X_GPIO,LEFT_X_PIN,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(LEFT_Y_GPIO,LEFT_Y_PIN,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(RIGHT_X_GPIO,RIGHT_X_PIN,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(RIGHT_Y_GPIO,RIGHT_Y_PIN,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(BAT_MON_GPIO,BAT_MON_PIN,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(POT_A_GPIO,POT_A_PIN,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(POT_B_GPIO,POT_B_PIN,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(POT_C_GPIO,POT_C_PIN,GPIO_Mode_In_FL_No_IT);
  GPIO_Init(POT_D_GPIO,POT_D_PIN,GPIO_Mode_In_FL_No_IT);
  // ADC_VrefintCmd(ENABLE);
  ADC_Init(ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1,ADC_Group_FastChannels,ADC_SamplingTime_48Cycles);
  ADC_DMACmd(ADC1,ENABLE);
  ADC_Cmd(ADC1,ENABLE);
  
  //--------!!!!!!channel selection must be done after enable the ADC, refer to manual page 209------------------------
  ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,ENABLE);      //REF 1.21V
  ADC_ChannelCmd(ADC1,ADC_Channel_0,ENABLE);          //Left X
  ADC_ChannelCmd(ADC1,ADC_Channel_1,ENABLE);         //Left Y
  ADC_ChannelCmd(ADC1,ADC_Channel_2,ENABLE);         //RIGHT X
  ADC_ChannelCmd(ADC1,ADC_Channel_3,ENABLE);         //RIGHT Y
  ADC_ChannelCmd(ADC1,ADC_Channel_4,ENABLE);         //Battery monitor
  ADC_ChannelCmd(ADC1,ADC_Channel_10,ENABLE);         //POT D
  ADC_ChannelCmd(ADC1,ADC_Channel_19,ENABLE);         //POT A
  ADC_ChannelCmd(ADC1,ADC_Channel_20,ENABLE);         //POT B
  ADC_ChannelCmd(ADC1,ADC_Channel_21,ENABLE);         //POT C
  
  //-----------Use timer2 to triger the ADC, also used as LCD PWM------------------------
  TIM2_DeInit();
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);
  TIM2_TimeBaseInit(TIM2_Prescaler_1,TIM2_CounterMode_Up,16000);//1000Hz TIM2_Cmd(ENABLE);
  TIM2_ITConfig(TIM2_IT_Update,ENABLE);
  TIM2_OC1Init(TIM2_OCMode_PWM1,TIM2_OutputState_Enable,8000,TIM2_OCPolarity_High,TIM2_OCIdleState_Reset);
  TIM2_CCxCmd(TIM2_Channel_1,ENABLE);
  TIM2_CtrlPWMOutputs(ENABLE);
  TIM2_Cmd(ENABLE);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1,ENABLE);
  DMA_Init(DMA1_Channel0,(uint32_t)ADC_ResultsRaw,ADC1_BASE+0x04,ADC_BUFFER_SIZE,DMA_DIR_PeripheralToMemory,DMA_Mode_Circular,DMA_MemoryIncMode_Inc,DMA_Priority_High,DMA_MemoryDataSize_HalfWord);
  DMA_ITConfig(DMA1_Channel0,DMA_ITx_TC,ENABLE);
  DMA_Cmd(DMA1_Channel0,ENABLE);
  DMA_GlobalCmd(ENABLE);
}

//Timer2 triger ADC in every 1ms
void TIM2_Update_ISR(void)
{
  ADC_SoftwareStartConv(ADC1);
}

void SYSTEM_VariablesUpdate(void)
{
  if(SYSTEM_ADC_UpdateFlag)
  {
    SYSTEM_ADC_UpdateFlag = RESET;
    
   
  }
 
}


void SYSTEM_ADC_DMA_ISR(void)
{
  static uint32_t temp_sum[ADC_BUFFER_SIZE];
  static uint8_t sum_count = 0;
  uint8_t cnt = 0;
  for(cnt = 0;cnt<ADC_BUFFER_SIZE;cnt++)
  {
    temp_sum[cnt] += ADC_ResultsRaw[cnt];
  }
  sum_count++;
  
  if(sum_count == ADC_AVERAGE_NUMBER)
  {
    sum_count = 0;
    for(cnt = 0;cnt<ADC_BUFFER_SIZE;cnt++)
    {
      ADC_Results[cnt] = temp_sum[cnt]/ADC_AVERAGE_NUMBER;
      temp_sum[cnt] = 0;
    }   
    SYSTEM_ADC_UpdateFlag = SET;
  }
}