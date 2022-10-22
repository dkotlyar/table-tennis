/*
 * step_motor.h
 *
 * Created: 19.09.2016 7:53:52
 *  Author: Dmitry
 */ 


#ifndef STEP_MOTOR_H_
#define STEP_MOTOR_H_

typedef struct __step_motor__ {
	uint8_t state;
	float current_position;
	int16_t target_position;

	uint8_t positioning;
	uint8_t direction;
	uint8_t enabled;
	uint8_t ready;
	uint8_t travel;
	uint8_t acceleration_enabled;
	uint8_t deceleration_enabled;
	
	uint16_t half_steps;
	uint16_t steps_count;
	
	float OCR_value;
	uint16_t OCR_start;
	uint16_t OCR_work;
	uint16_t OCR_finish;
	
	uint16_t steps_acceleration;
	uint16_t steps_deceleration;
	
	uint16_t start_freq;
	uint16_t work_freq;
	uint16_t finish_freq;

	int16_t minimum_position;
	int16_t maximum_position;
	float distance_per_step;
	
} step_motor_t;

void step_motor_init(step_motor_t * motor);
uint16_t step_motor_freq2ticks(step_motor_t * motor, uint16_t freq);
void step_motor_goto(step_motor_t * motor, int16_t position);
void step_motor_positioning(step_motor_t * motor);
void step_motor_enable(step_motor_t * motor);
void step_motor_disable(step_motor_t * motor);
void step_motor_stop(step_motor_t * motor);
void step_motor_start(step_motor_t * motor);
uint8_t step_motor_is_max(step_motor_t * motor);
uint8_t step_motor_is_min(step_motor_t * motor);
void step_motor_set_dir(step_motor_t * motor, uint8_t direction);
void step_motor_change_speed(step_motor_t * motor);

void step_motor_acceleration_enable(step_motor_t * motor);
void step_motor_acceleration_disable(step_motor_t * motor);
void step_motor_deceleration_enable(step_motor_t * motor);
void step_motor_deceleration_disable(step_motor_t * motor);

void step_motor_led_min(step_motor_t * motor);
void step_motor_led_max(step_motor_t * motor);
void step_motor_led_disable(step_motor_t * motor);

uint8_t step_motor_check_ready(step_motor_t * motor);
uint8_t step_motor_is_travel(step_motor_t * motor);



#endif /* STEP_MOTOR_H_ */