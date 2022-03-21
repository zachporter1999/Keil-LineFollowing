#include <MKL25Z4.h>
#include <math.h>
#include "ir_sensor.h"
#include "LEDs.h"
#include "adc.h"
#include "motor.h"
#include "timers.h"

#define N_SENSORS 7
#define ANGLE_LEFT_TURN 0
#define ANGLE_RIGHT_TURN 80
#define MAX_ERROR 90
#define DUTY_MIN 30

#define INTERRUPT_PORT PORTE
#define INTERRUPT_PT   PTE
#define INTERRUPT_PIN  31
#define INTERRUPT_SCGC SIM_SCGC5_PORTE_MASK

void delay(uint32_t dly)
{
	volatile uint32_t t;
	for (t = dly * 10000; t > 0; t--);
}

void set_turn_pwm(double diff){
	// adjust vduty based on how Set_PWM_Value_Ch1 works
	uint8_t duty = fabs(diff) /RANGE * 100; //double check this conversion
	if (duty < DUTY_MIN){
		duty = DUTY_MIN;
	}
	if(diff < 0){
		Set_Forward(duty);
		control_RGB_LEDs(1,0,0);
	}else{
		Set_Reverse(duty);
		control_RGB_LEDs(0,1,0);
	}
}

void setup_interrupt_pin()
{
	SIM->SCGC5 |= INTERRUPT_SCGC;
	
	INTERRUPT_PORT->PCR[INTERRUPT_PIN] &= ~(uint32_t)(PORT_PCR_MUX(1));
	
	INTERRUPT_PT->PDDR |= (uint32_t)(1 << INTERRUPT_PIN);
	INTERRUPT_PT->PCOR |= (uint32_t)(1 << INTERRUPT_PIN);
}

void send_interrupt_signal()
{
	INTERRUPT_PT->PSOR |= (uint32_t)(1 << INTERRUPT_PIN);
	delay(100);
	INTERRUPT_PT->PCOR |= (uint32_t)(1 << INTERRUPT_PIN);
}

int main(void)
{	

	double new_angle = 0;
	double current_angle = 0;
	double difference = 0;
	double test = 0;
	char lcd_out[10];
	int counter = 0;
	
	ir_cfg_t ir_cfg[N_SENSORS] = {
		// Left
		IR_PORTA_CFG(17),
		IR_PORTC_CFG(10),
		IR_PORTC_CFG(6),
		IR_PORTC_CFG(5),
		IR_PORTC_CFG(4),
		IR_PORTC_CFG(3),
		IR_PORTC_CFG(0)
		// Right
	};
	
	init_ir_array(ir_cfg, N_SENSORS);
	init_RGB_LEDs();
	Init_ADC();
	// init array
	Init_Drive_Motor();
	Init_PWM();

	
	for (;;)
	{
		// read arraY
		
		current_angle = Get_Position();
		new_angle = get_angle(ir_cfg, N_SENSORS, LINE_DETECTED_HIGH, ANGLE_LEFT_TURN, ANGLE_RIGHT_TURN);
		
		//block of code if no line is found
		if(new_angle < 0){
			Set_Stop();
			control_RGB_LEDs(0, 0, 1);
			counter ++;
			if (counter > MAX_ERROR){
				//send stop signal here as assume line has been lost
			}
			continue;
		}
		counter = 0;

		difference = new_angle - current_angle;
		test = fabs (difference);
		if(test<5){//if difference is within 5 degrees stop turning
			Set_Stop();
			continue;
		}
		// if steering is already close to max angle stop motor
		if((difference < 0 && current_angle <2.5) || (difference > 0 && current_angle >77.5)){
			Set_Stop();
			continue;
		}

		set_turn_pwm(difference);
	}
	
		
}
