/*
 * step_motor.c
 *
 * Created: 19.09.2016 7:53:40
 *  Author: Dmitry
 */ 

#include "main.h"
#include "step_motor.h"

void step_motor_halfstep(step_motor_t * motor) {
	if (motor->positioning)
		return;

	if (motor->direction) {
		motor->current_position += motor->distance_per_step / 2;
	} else {
		motor->current_position -= motor->distance_per_step / 2;
	}

	if ((int16_t)motor->current_position == motor->target_position) {
		step_motor_stop(motor);
		motor->ready = 1;
	}

	motor->half_steps++;
	if (motor->half_steps / 2 == motor->steps_count - motor->steps_deceleration) {
		step_motor_deceleration_enable(motor);
	}
}

void step_motor_init(step_motor_t * motor) {

}

uint16_t step_motor_freq2ticks(step_motor_t * motor, uint16_t freq) {

}

void step_motor_goto(step_motor_t * motor, int16_t position) {
	if (position < motor->minimum_position) {
		position = motor->minimum_position;
	} else if (position > motor->maximum_position) {
		position = motor->maximum_position;
	}

	if (motor->target_position == position) {
		return;
	}

	if (motor->positioning) {
		motor->target_position = position;
		return;
	}

	uint8_t new_direction = position > motor->target_position;
	if (new_direction != motor->direction) {
		step_motor_stop(motor);
		_delay_ms(50);
		step_motor_set_dir(motor, new_direction);
	}

	motor->steps_count = (uint16_t)round(abs(motor->current_position - position) / motor->distance_per_step);

	step_motor_acceleration_enable(motor);
	step_motor_start(motor);

	motor->target_position = position;
}

void step_motor_positioning(step_motor_t * motor) {

}

void step_motor_enable(step_motor_t * motor) {

}

void step_motor_disable(step_motor_t * motor) {

}

void step_motor_stop(step_motor_t * motor) {

}

void step_motor_start(step_motor_t * motor) {

}

uint8_t step_motor_is_max(step_motor_t * motor) {

}

uint8_t step_motor_is_min(step_motor_t * motor) {

}

void step_motor_set_dir(step_motor_t * motor, uint8_t direction) {

}

void step_motor_change_speed(step_motor_t * motor) {

}

void step_motor_acceleration_enable(step_motor_t * motor) {

}

void step_motor_acceleration_disable(step_motor_t * motor) {

}

void step_motor_deceleration_enable(step_motor_t * motor) {

}

void step_motor_deceleration_disable(step_motor_t * motor) {

}

void step_motor_led_min(step_motor_t * motor) {

}

void step_motor_led_max(step_motor_t * motor) {

}

void step_motor_led_disable(step_motor_t * motor) {

}

uint8_t step_motor_check_ready(step_motor_t * motor) {

}

uint8_t step_motor_is_travel(step_motor_t * motor) {

}