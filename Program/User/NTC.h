#ifndef NTC_H
#define NTC_H

#define NTC100K_R_PULL 120000
#define NTC10K_R_PULL 16000

extern uint8_t NTC100K_Temperature_Offset;
extern uint16_t NTC100K_Temperature;

uint16_t NTC_ResistorToTemperature(uint16_t const *table,uint32_t resistor);
uint16_t NTC_VoltageToResistor(uint16_t reference,uint16_t r_pull,uint16_t voltage);
uint16_t NTC_CountToTemperature(uint32_t r_pullup,uint16_t count);

#endif

