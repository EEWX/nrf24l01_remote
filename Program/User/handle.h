#ifndef HANDLE_H
#define HANDLE_H

#define HANDLE_CHANGE_GPIO GPIOC
#define HANDLE_CHANGE_PIN GPIO_Pin_5

#define HANDLE_SLEEP_GPIO GPIOC
#define HANDLE_SLEEP_PIN GPIO_Pin_6

typedef struct HandleStruct
{
  uint16_t Handle_ID;
  uint16_t WorkingSetpoint;
  uint16_t SleeepingSetpoint;
}
Handle_Typedef;

void HANDLE_Init(void);
void SYSTEM_HandleControl(void);
void HANDLE_PWM_Set(int32_t pwm_in);
void HANDLE_SleepDetect(void);

#endif