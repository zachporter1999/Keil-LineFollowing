#include "motor.h"
void Init_Drive_Motor(){
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK;

	//set to GPIO
	PIN_INA1_PORT->PCR[PIN_INA1_SHIFT] &= ~PORT_PCR_MUX_MASK;          
	PIN_INA1_PORT->PCR[PIN_INA1_SHIFT] |= PORT_PCR_MUX(1); 
	PIN_INA1_PT->PDDR |= PIN_INA1; //output
	
	
	//set to GPIO
	PIN_INB1_PORT->PCR[PIN_INB1_SHIFT] &= ~PORT_PCR_MUX_MASK;          
	PIN_INB1_PORT->PCR[PIN_INB1_SHIFT] |= PORT_PCR_MUX(1); 
	PIN_INB1_PT->PDDR |= PIN_INB1; //output
}

void Set_Stop(){
	PIN_INA1_PT->PCOR |= PIN_INA1;
	PIN_INB1_PT->PCOR |= PIN_INB1;
	Set_PWM_Value_Ch1(0);
}


void Set_Forward(uint8_t duty_cycle){
	PIN_INA1_PT->PCOR |= PIN_INA1;
	PIN_INB1_PT->PSOR |= PIN_INB1;
	Set_PWM_Value_Ch1(duty_cycle);
}

void Set_Reverse(uint8_t duty_cycle){
	PIN_INA1_PT->PSOR |= PIN_INA1;
	PIN_INB1_PT->PCOR |= PIN_INB1;
	Set_PWM_Value_Ch1(duty_cycle);
}
