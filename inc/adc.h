
#define ADC_H
#include "MKL25Z4.h"

#define VREF (3.3)
//WRITE VOLTAGE RANGE HERE
#define MINVoltage (##)
#define MAXVoltage (##)
#define RANGE (80)
#define VoltSloap ((MaxVoltage-MinVoltage)/ RANGE)

void Init_ADC(void);
double Get_Position();
void test();