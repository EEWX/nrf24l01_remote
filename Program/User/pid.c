#include "stm8l15x.h"
#include "pid.h"
#include "handle.h"
#include "system.h"

float Kp = DEFAULT_PID_P;
float Ki = DEFAULT_PID_I;
float Kd = DEFAULT_PID_D;

long PID_Control(int set,int readback)
{
  int error = 0;
  static int last_error = 0;
  static float integral = 0;
  static int derivative = 0;
  float pid_out = 0;
  last_error = error;
  error = set - readback;
  integral = integral + error;
  derivative = error - last_error;
  pid_out = (Kp * error) + (Ki * integral) + (Kd * derivative);
  if(pid_out > HANDLE_PWM_PEROID_COUNT)
  {
    pid_out = HANDLE_PWM_PEROID_COUNT;
  }
  
  if(pid_out < 0)
  {
    pid_out = 0;
  }
  return(pid_out);
}