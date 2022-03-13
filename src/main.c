#include <MKL25Z4.h>
#include "ir_sensor.h"

#define N_SENSORS 4

int main(void)
{	
	float line_position = 0;
	ir_cfg_t ir_cfg[N_SENSORS] = {IR_PORTC_CFG(3), IR_PORTC_CFG(4), IR_PORTC_CFG(5), IR_PORTC_CFG(6)};
	
	// init array
	init_ir_array(ir_cfg, N_SENSORS);
	
	for (;;)
	{
		// read arraY
		line_position = get_position(ir_cfg, N_SENSORS, LINE_DETECTED_LOW);
	}
}
