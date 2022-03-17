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


void set_turn_pwm(double diff){
	// adjust vduty based on how Set_PWM_Value_Ch1 works
	uint8_t duty = fabs(diff) /RANGE * 100; //double check this conversion
	if(diff < 0){
		Set_Forward(duty);
		control_RGB_LEDs(1,0,0);
	}else{
		Set_Reverse(duty);
		control_RGB_LEDs(0,1,0);
	}
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
		IR_PORTA_CFG(17),
		IR_PORTC_CFG(10),
		IR_PORTC_CFG(6),
		IR_PORTC_CFG(5),
		IR_PORTC_CFG(4),
		IR_PORTC_CFG(3),
		IR_PORTC_CFG(0)
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
		if((difference < 0 && current_angle <5) || (difference > 0 && current_angle >75)){
			Set_Stop();
			continue;
		}

		set_turn_pwm(difference);
	}
	
		
}
