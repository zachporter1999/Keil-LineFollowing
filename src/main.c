#include <MKL25Z4.h>
#include <math.h>
#include "ir_sensor.h"
#include "LEDs.h"
#include "adc.h"
#include "motor.h"
#include "timers.h"

#define N_SENSORS 4
#define ANGLE_LEFT_TURN 20
#define ANGLE_RIGHT_TURN 60

void set_turn_pwm(double diff){
	// adjust vduty based on how Set_PWM_Value_Ch1 works
	uint8_t duty = fabs(diff) /RANGE * 100; //double check this conversion
	if(diff < 0){
		Set_Reverse(duty);
	}else{
		Set_Forward(duty);
	}
}

int main(void)
{	
	double line_position = 0;
	double current_angle = 0;
	double difference = 0;
	int counter = 0;
	ir_cfg_t ir_cfg[N_SENSORS] = {IR_PORTC_CFG(3), IR_PORTC_CFG(4), IR_PORTC_CFG(5), IR_PORTC_CFG(6)};
	
	init_RGB_LEDs();
	Init_ADC();
	// init array
	init_ir_array(ir_cfg, N_SENSORS);
	Init_Drive_Motor();
	
	for (;;)
	{
		// read arraY
		line_position = get_angle(
											ir_cfg,
											N_SENSORS,
											LINE_DETECTED_HIGH,
											ANGLE_LEFT_TURN, 
											ANGLE_RIGHT_TURN);
		
		current_angle = Get_Position();

		//block of code if no line is found
		if(current_angle < 0){
			Set_Stop()
			counter ++;
			if (counter > MAX_ERROR){
				//send stop signal here as assume line has been lost
			}
			continue
		}
		counter = 0;

		difference = line_position - angle;

		if(fabs(difference)<5){//if difference is within 5 degrees stop turning
			Set_Stop();
		}
		// if steering is already close to max angle stop motor
		if(difference < 0 && current_angle <5){
			Set_Stop();
		}else if(difference > 0 && current_angle >75){
			Set_Stop();
		}

		set_turn_pwm(difference)		

		// led for live debugging
		// if (line_position < 0)
		// {
		// 	control_RGB_LEDs(1, 0, 0);
		// }
		// else if (line_position >= (0.6 * ((ANGLE_RIGHT_TURN - ANGLE_LEFT_TURN) + ANGLE_LEFT_TURN)))
		// {
		// 	control_RGB_LEDs(0, 0, 1);
		// }
		// else if (line_position <= (0.4 * ((ANGLE_RIGHT_TURN - ANGLE_LEFT_TURN) + ANGLE_LEFT_TURN)))
		// {
		// 	control_RGB_LEDs(0, 1, 0);
		// }
		// else 
		// {
		// 	control_RGB_LEDs(0, 1, 1);
		// }
	}
}
