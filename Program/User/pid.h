#ifndef PID_H
#define PID_H

#define DEFAULT_PID_P 3000               //5000
#define DEFAULT_PID_I 200                 //10
#define DEFAULT_PID_D 200            //10

extern float Kp;
extern float Ki;
extern float Kd;

long PID_Control(int set,int readback);

#endif
