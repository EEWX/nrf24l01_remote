#include "stm8l15x.h"
#include "ntc.h"

//-------------TE 10K3A1--------------------
const uint16_t NTC_10K_TABLE[111]=
{
    32651,//0
    31031,
    29500,
    28054,
    26687,
    25395,
    24172,
    23016,
    21921,
    20885,
    19903,//10
    18973,
    18092,
    17257,
    16465,
    15714,
    15001,
    14324,
    13682,
    13073,
    12493,//20
    11943,
    11420,
    10923,
    10450,
    10000,
    9572,
    9165,
    8777,
    8408,
    8056,//30
    7721,
    7402,
    7097,
    6807,
    6530,
    6266,
    6014,
    5774,
    5544,
    5325,//40
    5116,//
    4916,
    4724,
    4542,
    4367,
    4200,
    4040,
    3887,
    3741,
    3601,//50
    3467,
    3339,
    3216,
    3098,
    2985,
    2877,
    2773,
    2674,
    2579,
    2487,//60
    2399,
    2315,
    2234,
    2157,
    2082,
    2011,
    1942,
    1876,
    1813,
    1752,//70
    1693,
    1637,
    1582,
    1530,
    1480,
    1432,
    1385,
    1341,
    1298,
    1256,//80
    1216,
    1178,
    1141,
    1105,
    1070,
    1037,
    1005,
    974,
    945,
    916,//90
    888,
    862,
    836,
    811,
    787,
    764,
    741,
    720,
    699,
    678,//100
    659,
    640,
    622,
    604,
    587,
    571,
    555,
    539,
    524,
    510 //110

};

//-------------MURATA NCP15WF104D03RC, valueX10--------------------
const uint16_t NTC_100K_TABLE[111]=
{
    35597,//0
    33710,
    31934,
    30262,
    28686,
    27201,
    25800,
    24480,
    23234,
    22059,
    20949,//10
    19901,
    18911,
    17976,
    17092,
    16256,
    15465,
    14718,
    14010,
    13340,
    12706,//20
    12105,
    11536,
    10996,
    10485,
    10000,
    9540,
    9103,
    8689,
    8296,
    7922,//30
    7568,
    7231,
    6910,
    6606,
    6317,
    6041,
    5780,
    5531,
    5293,
    5068,//40
    4853,//
    4648,
    4453,
    4267,
    4090,
    3921,
    3760,
    3606,
    3459,
    3319,//50
    3186,
    3058,
    2936,
    2820,
    2709,
    2602,
    2501,
    2404,
    2311,
    2222,//60
    2137,
    2056,
    1978,
    1904,
    1832,
    1764,
    1698,
    1636,
    1576,
    1518,//70
    1463,
    1410,
    1359,
    1310,
    1263,
    1219,
    1176,
    1134,
    1095,
    1056,//80
    1020,
    985,
    951,
    918,
    887,
    857,
    828,
    801,
    774,
    748,//90
    723,
    700,
    677,
    655,
    634,
    613,
    593,
    574,
    556,
    538,//100
    521,
    505,
    489,
    474,
    459,
    445,
    431,
    418,
    406,
    393 //110

};



uint16_t NTC_ResistorToTemperature(uint16_t const *table,uint32_t resistor)
{
    uint8_t index;
    uint16_t temp = 0xffff;
    uint8_t percent;
    for(index=0;index<109;index++)
    {
        if(resistor == *(table+index))
        {
            temp = index*10;
            break;
        }
        else if(resistor<*(table+index) && resistor>*(table+index+1))
        {
            percent = (*(table+index) - resistor)*10/(*(table+index) - *(table+index+1));
            temp = index*10 + percent;
            break;
        }
        
    }
    return(temp);
}


//------------voltage is 1000 times bigger
uint16_t NTC_VoltageToResistor(uint16_t reference,uint16_t r_pull,uint16_t voltage)
{
    uint32_t temp = voltage;
    return(r_pull*temp/(reference-temp));
}

//---------------used for condition where sensor divider voltage and vref are same, adc 12bit-----------------
uint16_t NTC_CountToTemperature(uint32_t r_pullup,uint16_t count)
{
  uint32_t temp = r_pullup;
  temp = temp*count/(4096-count);
  if(r_pullup < 100000)
  {
    return(NTC_ResistorToTemperature(NTC_10K_TABLE,temp));
  }
  else
  {
    return(NTC_ResistorToTemperature(NTC_100K_TABLE,temp/10));
  }
  
}