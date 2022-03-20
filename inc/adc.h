
#define ADC_H
#include "MKL25Z4.h"

#define VREF (3.3)
//WRITE VOLTAGE RANGE HERE
#define MINVoltage (1.13)
#define MAXVoltage (2.24)
#define RANGE (80)
#define VoltSloap ((MAXVoltage-MINVoltage)/ RANGE)

void Init_ADC(void);
double Get_Position();
void test();