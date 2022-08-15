#include "stm8l15x.h"
#include "handle.h"
#include "thermalcouple.h"
#include "pid.h"


uint16_t HANDLE_TemperatureRaw = 0;
uint16_t HANDLE_Temperature = 0;
uint16_t HANDLE_TipTemperatureRaw = 0;
uint16_t HANDLE_TipTemperature = 0;
uint16_t HANDLE_CurrentRaw = 0;
uint16_t HANDLE_Current = 0;
uint16_t HANDLE_VoltageRaw = 0;
uint16_t HANDLE_Voltage = 0;
uint16_t HANDLE_ID = 0;
uint16_t HANDLE_Reference = 0;
uint16_t HANDLE_TipSetpoint = 150;
FlagStatus HANDLE_UpdateFlag = RESET;

void HANDLE_Init(void)
{
  GPIO_Init(HANDLE_SLEEP_GPIO,HANDLE_SLEEP_PIN,GPIO_Mode_In_PU_No_IT);                     
  GPIO_Init(HANDLE_CHANGE_GPIO,HANDLE_CHANGE_PIN,GPIO_Mode_In_PU_No_IT);                     
  
}







void HANDLE_SleepDetect(void)
{
  static int8_t debounce_cnt = 0;
  static uint16_t setpoint_reg = 0;
  static FlagStatus handle_sleep = SET;
  static FlagStatus handle_sleep_last = SET;
  
  if(GPIO_ReadInputDataBit(HANDLE_SLEEP_GPIO,HANDLE_SLEEP_PIN))
  {
    debounce_cnt++;
    if(debounce_cnt > 20)
    {
      debounce_cnt = 0;
      handle_sleep = RESET;
    }
  }
  else
  {
    debounce_cnt--;
    if(debounce_cnt<-20)
    {
      debounce_cnt = 0;
      handle_sleep = SET;
    }
  }
  
  if(handle_sleep_last != handle_sleep)
  {
    handle_sleep_last = handle_sleep;
    if(handle_sleep)
    {
       setpoint_reg = HANDLE_TipSetpoint;
     //  HANDLE_TipSetpoint = DEFAULT_HANDLE_SLEEP_SETPOINT;
      // JBC_Windows = Window_Sleeping;
    }
    else
    {
     // if(setpoint_reg > DEFAULT_HANDLE_SLEEP_SETPOINT)
    //  {
      //  HANDLE_TipSetpoint = setpoint_reg;
        
    //  }
     // JBC_Windows = Window_Working;
    }
  }
  
}






