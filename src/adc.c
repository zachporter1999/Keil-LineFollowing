#include <MKL25Z4.H>
#include "LEDs.h"
#include "adc.h"

void Init_ADC(void) {
	
	SIM->SCGC6 |= (1UL << SIM_SCGC6_ADC0_SHIFT); 
	ADC0->SC3 &= ~(1UL << ADC_SC3_AVGE_MASK);//double check this
	ADC0->CFG1 = 0x0C; // Select 16 bit resolution
	ADC0->SC2 = 0;
	ADC0->CFG2|= 0x3;

}

double Get_Position(){
	volatile double res=0;
	volatile double voltage, angle;
	ADC0->SC1[0] = 0x00; // start conversion on channel 0 PTE20
	
	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
		;
	res = ADC0->R[0];
	voltage = VREF*(res/0xffff);
	//uncomment when voltage range is found
	angle = ((voltage-MINVoltage) / VoltSloap);
	//return voltage;
	return angle;
}


