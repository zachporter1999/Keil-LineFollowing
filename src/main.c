#include <MKL25Z4.h>
#include "ir_sensor.h"
#include "LEDs.h"

#define N_SENSORS 4
#define ANGLE_LEFT_TURN 20
#define ANGLE_RIGHT_TURN 60

int main(void)
{	
	double line_position = 0;
	ir_cfg_t ir_cfg[N_SENSORS] = {IR_PORTC_CFG(3), IR_PORTC_CFG(4), IR_PORTC_CFG(5), IR_PORTC_CFG(6)};
	
	init_RGB_LEDs();
	
	// init array
	init_ir_array(ir_cfg, N_SENSORS);
	
	for (;;)
	{
		// read arraY
		line_position = get_angle(
											ir_cfg,
											N_SENSORS,
											LINE_DETECTED_HIGH,
											ANGLE_LEFT_TURN, 
											ANGLE_RIGHT_TURN);
		
		// led for live debugging
		if (line_position < 0)
		{
			control_RGB_LEDs(1, 0, 0);
		}
		else if (line_position >= (0.6 * ((ANGLE_RIGHT_TURN - ANGLE_LEFT_TURN) + ANGLE_LEFT_TURN)))
		{
			control_RGB_LEDs(0, 0, 1);
		}
		else if (line_position <= (0.4 * ((ANGLE_RIGHT_TURN - ANGLE_LEFT_TURN) + ANGLE_LEFT_TURN)))
		{
			control_RGB_LEDs(0, 1, 0);
		}
		else 
		{
			control_RGB_LEDs(0, 1, 1);
		}
	}
}
