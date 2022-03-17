#ifndef __IR_SENSOR_H
#define __IR_SENSOR_H

#include <MKL25Z4.h>

/*
 * Use these to define which ports and pins the ir sensors are wired to. 
 * eg.
 *		To initialize a ir sensor on port A, pin 17.
 *
 *    ir_cfg_t ir_sensor = IR_PORTA_CFG(17);
 *    init_ir(&ir_sensor);
 */
#define IR_PORTA_CFG(pin) (ir_cfg_t){PORTA, PTA, SIM_SCGC5_PORTA_MASK, pin}
#define IR_PORTB_CFG(pin) (ir_cfg_t){PORTB, PTB, SIM_SCGC5_PORTB_MASK, pin}
#define IR_PORTC_CFG(pin) (ir_cfg_t){PORTC, PTC, SIM_SCGC5_PORTC_MASK, pin}
#define IR_PORTD_CFG(pin) (ir_cfg_t){PORTD, PTD, SIM_SCGC5_PORTD_MASK, pin}
#define IR_PORTE_CFG(pin) (ir_cfg_t){PORTE, PTE, SIM_SCGC5_PORTE_MASK, pin}

/*
 * Macros for indicating what level the IR sensor outputs when a line is detected
 */
#define LINE_DETECTED_LOW  0
#define LINE_DETECTED_HIGH 1

/*
 * ir_cfg_t contains the PORTx, PTx, SIM_SCGCx, and pin number for gpio config.
 * 
 * This structure is used to pass all the necessary config info to the below funcitons.
 */
typedef struct
{
	PORT_Type* port;
	GPIO_Type* pt;
	uint32_t scgc_mask;
	uint32_t pin;
} ir_cfg_t;

/*
 * init_ir
 *
 * Initializes GPIO for input without interrupts.
 *
 * Params:
 *   *cfg - ir_cfg_t corresponding to the pin to be configured.
 */
void init_ir(ir_cfg_t* cfg);

/*
 * read_ir
 *
 * Reads the GPIO pin.
 *
 * Params:
 *   *cfg         - ir_cfg_t corresponding to the pin to be read.
 *   active_state - The level of the GPIO pin indicating the sensor has detected a line. (0 or 1).
 * 
 * Return: 
 *   1 when the IR sensor detects the line.
 *   0 when the IR sensor does not detect the line.
 */
uint8_t read_ir(ir_cfg_t* cfg, int active_state);

/*
 * init_ir_array
 *
 * Initializes an array of ir sensors.
 *
 * Params:
 *   *a_cfg - The array of ir_cfg_t's to initialize sensors.
 *    n_cfg - The number of cfg's in *a_cfg.
 */
void init_ir_array(ir_cfg_t* a_cfg, uint8_t n_cfg);

/*
 * get_position
 *
 * Calculates the position of the line under the IR sensor array.
 *
 * Params:
 *   *a_cfg            - The array of ir_cfg_t's to initialize sensors.
 *    n_cfg            - The number of cfg's in *a_cfg.
 *    active_state     - The level of the GPIO pin indicating the sensor has detected a line. (0 or 1).
 *    angle_left_turn  - Limit for how far left the motor steers. (In an ideal case this angle should ne 0 degrees)
 *    angle_right_turn - Limit for how far right the motor steers. (In an ideal case this angle should be 80 dgrees)
 *
 */
float get_angle(
	ir_cfg_t* a_cfg,
	uint8_t n_cfg,
	int active_state,
	float angle_left_turn,
	float angle_right_turn
);

#endif 
