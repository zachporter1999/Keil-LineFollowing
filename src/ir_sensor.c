#include "ir_sensor.h"

void init_ir(ir_cfg_t* cfg)
{
	SIM->SCGC5 |= cfg->scgc_mask;
	
	cfg->port->PCR[cfg->pin] |= PORT_PCR_MUX(1);
	
	cfg->pt->PDDR &= ~(uint32_t)(1 << cfg->pin);
}

uint8_t read_ir(ir_cfg_t* cfg, int active_state)
{
	return (uint8_t)active_state ^ ((cfg->pt->PDIR & (1 << cfg->pin)) > 0);
}

void init_ir_array(ir_cfg_t* a_cfg, uint8_t n_cfg)
{
	for (int i = 0; i < n_cfg; i++)
	{
		init_ir(&a_cfg[i]);
	}
}

float get_angle(
	ir_cfg_t* a_cfg,
	uint8_t n_cfg,
	int active_state,
	float angle_left_turn,
	float angle_right_turn
)
{
	uint32_t sum = 0;
	uint32_t weighted_sum = 0;
	
	for (int i = 0; i < n_cfg; i++)
	{
		uint8_t sensor_res = read_ir(&a_cfg[i], active_state);
		
		sum += sensor_res;
		weighted_sum += (uint8_t)(i + 1) * sensor_res;
	}
	
	if (sum == 0)
	{
		return -1;
	}
	
	float line_position = (float)(weighted_sum / sum);
	
	return (line_position - 1) * (angle_right_turn - angle_left_turn) / (n_cfg - 1) + angle_left_turn;
}
