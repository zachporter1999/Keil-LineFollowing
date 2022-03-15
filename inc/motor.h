#ifndef MOTOR_H
#define MOTOR_H

#include <MKL25Z4.h>
#include "LEDS.h"
#include "timers.h"

#define PIN_INA1_PORT	PORTA
#define PIN_INA1_PT	PTA
#define PIN_INA1_SHIFT  ( 2 )
#define PIN_INA1	(1 << PIN_INA1_SHIFT)

#define PIN_INB1_PORT	PORTA
#define PIN_INB1_PT	PTA
#define PIN_INB1_SHIFT  ( 4 )
#define PIN_INB1        ( 1 << PIN_INB1_SHIFT)

void Init_Drive_Motor();
void Set_Stop();
void Set_Forward(uint8_t duty_cycle);
void Set_Reverse(uint8_t duty_cycle);


#endif