#ifndef THERMALCOUPLE_H
#define THERMALCOUPLE_H

#define DEFAULT_THERMALCOUPLE_GAIN 152.63          //152.6
#define DEFAULT_THERMALCOUPLE_OFFSET 0.0022          //0.0022

float THERMALCOUPLE_VoltageRecovery(float origin_volt);
float THERMALCOUPLE_VoltageToTemperature(float thermal_volt);
//-------------in uV-------------------
float THERMALCOUPLE_Update(float volt);

#endif