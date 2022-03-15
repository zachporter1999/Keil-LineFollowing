#include "timers.h"
#include "MKL25Z4.h"
#include <stdlib.h>

volatile unsigned PIT_interrupt_counter = 0;
volatile unsigned LCD_update_requested = 0;

extern volatile uint8_t hour, minute, second;
extern volatile uint16_t millisecond;

void Init_PIT(unsigned period) {
	// Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	
	// Initialize PIT0 to count down from argument 
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);

	// No chaining
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	// Generate interrupts
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

	/* Enable Interrupts */
	NVIC_SetPriority(PIT_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	NVIC_EnableIRQ(PIT_IRQn);	
}


void Start_PIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void Stop_PIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}


void PIT_IRQHandler() {
	static unsigned LCD_update_delay = LCD_UPDATE_PERIOD;

	//clear pending IRQ
	NVIC_ClearPendingIRQ(PIT_IRQn);
	
	// check to see which channel triggered interrupt 
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		
		// Do ISR work
		millisecond++;
		if (millisecond > 999) {
			millisecond = 0;
			second++;
			if (second > 59) {
				second = 0;
				minute++;
				if (minute > 59) {
					minute = 0;
					hour++;
				}
			}
		}

		// light LED in first portion of each second
		if (millisecond < 600) {
			Set_PWM_Value_Ch1(millisecond/6);
			Set_PWM_Value_Ch0(millisecond/6);
		}
		
				
	} else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	} 
}

void Init_PWM()
{
	//turn on clock to TPM 
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK;
	
	//set clock source for tpm
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	//load the counter and mod
	TPM1->MOD = PWM_MAX_COUNT;
	TPM2->MOD = 65535;
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	//PORTA->PCR[12] = PORT_PCR_MUX(3);
	PORTA->PCR[13] = PORT_PCR_MUX(3);
	PORTA->PCR[1] = PORT_PCR_MUX(3);
	
	
	
	
		
	//set channels to center-aligned high-true PWM
	//TPM1->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	TPM1->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	
	//TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	//set TPM to up-down and divide by 8 prescaler and clock mode
	TPM1->SC = (TPM_SC_CPWMS_MASK | TPM_SC_CMOD(1) | TPM_SC_PS(3));
	//TPM2->SC = (TPM_SC_CMOD(1) | TPM_SC_PS(1));
	//set trigger mode
	TPM1->CONF |= TPM_CONF_TRGSEL(0x8);
	//TPM2->CONF |= TPM_CONF_TRGSEL(0x8);
	
	//TPM1->CONTROLS[0].CnV = PWM_MAX_COUNT/2;
	TPM1->CONTROLS[1].CnV = PWM_MAX_COUNT;
	//TPM2->CONTROLS[0].CnV = 36000;
}


void Set_PWM_Value_Ch0(uint8_t duty_cycle) {
	uint16_t n;
	
	n = duty_cycle*PWM_MAX_COUNT/100; 
  TPM1->CONTROLS[0].CnV = n;
}

void Set_PWM_Value_Ch1(uint8_t duty_cycle) {
	uint16_t n;
	
	n = duty_cycle*PWM_MAX_COUNT/100; 
  TPM1->CONTROLS[1].CnV = n;
}


void Set_PWM_Servo(uint8_t degree){
	uint16_t n;
	if (degree > 180 || degree < 90){
		return;
	}
	
	n = degree * 177 + 12000;
	TPM2->CONTROLS[0].CnV = n;
}
// *******************************ARM University Program Copyright � ARM Ltd 2013*************************************   
