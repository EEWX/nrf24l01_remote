#include "stm8l15x.h"
#include "thermalcouple.h"



//---------ThermalCoupleTypeKTable[0] --> 0C
//---------ThermalCoupleTypeKTable[1] --> 10C
//---------ThermalCoupleTypeKTable[10] --> 100C
const uint16_t ThermalCoupleTypeKTable[51]=
{
0,
397,            //397uV
798,
1203,
1612,
2023,
2436,
2851,
3267,
3682,
4096,
4509,
4920,
5328,
5735,
6138,
6540,
6941,
7340,
7739,
8138,
8539,
8940,
9343,
9747,
10153,          //10.153mV
10561,
10971,
11382,
11795,
12209,
12624,
13040,
13457,
13874,
14293,
14713,
15133,
15554,
15975,
16397,
16820,
17243,
17667,
18091,
18516,
18941,
19366,
19792,
20218,
20644
};
//-------------------------volt:in uV---------------
float THERMALCOUPLE_VoltageToTemperature(float thermal_volt)
{
    uint8_t thermal_index;
    float table_position = 0;
    float thermal_percent = 0;
    float thermal_volt_temp = thermal_volt * 1000000;
    for(thermal_index=0;thermal_index<51;thermal_index++)
    {
        if(thermal_volt_temp == ThermalCoupleTypeKTable[thermal_index])
        {
            table_position = thermal_index;
            break;
        }
        else if(thermal_volt_temp<ThermalCoupleTypeKTable[thermal_index+1] && thermal_volt_temp>ThermalCoupleTypeKTable[thermal_index])
        {
            thermal_percent = ((thermal_volt_temp - ThermalCoupleTypeKTable[thermal_index]))/(ThermalCoupleTypeKTable[thermal_index+1] - ThermalCoupleTypeKTable[thermal_index]);
            table_position = thermal_index+thermal_percent;
            break;
        }
    }
    return(table_position*10);
}

//---------------origin_volt in V --------------
float THERMALCOUPLE_VoltageRecovery(float origin_volt)
{
  return((origin_volt+DEFAULT_THERMALCOUPLE_OFFSET)/DEFAULT_THERMALCOUPLE_GAIN);
}

float THERMALCOUPLE_Update(float volt)
{
  float recovered_volt;
  recovered_volt = THERMALCOUPLE_VoltageRecovery(volt);
  return(THERMALCOUPLE_VoltageToTemperature(recovered_volt));
}
