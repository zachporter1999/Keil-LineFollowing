#ifndef __IR_SENSOR_H
#define __IR_SENSOR_H

#include <MKL25Z4.h>

// Use these to define which ports and pins the ir sensors are wired to. 
// eg.
//		ir sensor is connected to PORTA pin 16. Use IR_PORTA_CFG(18)
#define IR_PORTA_CFG(pin) (ir_cfg_t){PORTA, PTA, SIM_SCGC5_PORTA_MASK, pin}
#define IR_PORTB_CFG(pin) (ir_cfg_t){PORTB, PTB, SIM_SCGC5_PORTB_MASK, pin}
#define IR_PORTC_CFG(pin) (ir_cfg_t){PORTC, PTC, SIM_SCGC5_PORTC_MASK, pin}
#define IR_PORTD_CFG(pin) (ir_cfg_t){PORTD, PTD, SIM_SCGC5_PORTD_MASK, pin}
#define IR_PORTE_CFG(pin) (ir_cfg_t){PORTE, PTE, SIM_SCGC5_PORTE_MASK, pin}

// Struct to hold ir config.
typedef struct
{
	PORT_Type* port;
	GPIO_Type* pt;
	uint32_t scgc_mask;
	uint32_t pin;
} ir_cfg_t;

// Init single ir sensot
void init_ir(ir_cfg_t* cfg);

// Read single ir sensor
uint8_t read_ir(ir_cfg_t* cfg);
uint8_t read_ir(ir_cfg_t* cfg, int active_state);

// init an array of sensors
// a_cfg -> Array of sensor cfg
void init_ir_array(ir_cfg_t* a_cfg, uint8_t n_cfg);

// Reads array and calculates line position
// a_cfg -> Array of sensor cfg
float get_position(ir_cfg_t* a_cfg, uint8_t n_cfg);
float get_position(ir_cfg_t* a_cfg, uint8_t n_cfg, int active_state);

#endif 
