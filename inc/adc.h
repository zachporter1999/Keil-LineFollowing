
#define ADC_H
#include "MKL25Z4.h"

#define VREF (3.3)
//WRITE VOLTAGE RANGE HERE
#define MINVoltage (0.973)
#define MAXVoltage (2.174)
#define RANGE (80)
#define VoltSloap ((MAXVoltage-MINVoltage)/ RANGE)

void Init_ADC(void);
double Get_Position();
void test();