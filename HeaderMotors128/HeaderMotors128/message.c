/*
 * message.c
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "message.h"

#include "cmd.h"
#include "led.h"
#include "usart.h"

#include "incline.h"
#include "rollrotation.h"

void command_led(uint8_t * data, uint8_t length);

void command_enable_motor(uint8_t * data, uint8_t length);
void command_disable_motor(uint8_t * data, uint8_t length);
void command_positioning_motor(uint8_t * data, uint8_t length);
void command_save_zero_motor(uint8_t * data, uint8_t length);
void command_clear_zero_motor(uint8_t * data, uint8_t length);

void command_incline(uint8_t * data, uint8_t length);
void command_rollrotation(uint8_t * data, uint8_t length);

void add_transmit(uint8_t * data, uint8_t length) {
	usart_add_transmit(data, length);
} 

void handle_message(uint8_t * data, uint8_t length) {
	void (*f)(uint8_t*,uint8_t);
		
	switch (*data) {
		case LED:
			f = command_led;
			break;
			
		case ENABLE_MOTOR:
			f = command_enable_motor;
			break;
		case DISABLE_MOTOR:
			f = command_disable_motor;
			break;
		case POSITIONING_MOTOR:
			f = command_positioning_motor;
			break;
		case SAVE_ZERO_MOTOR:
			f = command_save_zero_motor;
			break;
		case CLEAR_ZERO_MOTOR:
			f = command_clear_zero_motor;
			break;
			
		case INCLINE:
			f = command_incline;
			break;
			
		case ROLLROTATION:
			f = command_rollrotation;
			break;
			
		
		default:
			return;
	}
	
	f(data+1, length-1);
}

void command_led(uint8_t * data, uint8_t length) {
	switch (*data) {
		case BLINK:
			led_blink(*(data+1));
			break;
		case ENABLE:
			led_enable();
			break;
		case DISABLE:
		default:
			led_disable();
			break;
	}
}

void command_enable_motor(uint8_t * data, uint8_t length) {
	switch (*data) {
		case ALL_MOTORS:
			incline_enable();
			rollrotation_enable();
			break;
		case MOTOR_INCLINE:
			incline_enable();
			break;
		case MOTOR_ROLLROTATION:
			rollrotation_enable();
			break;
	}
}

void command_disable_motor(uint8_t * data, uint8_t length) {
	switch (*data) {
		case ALL_MOTORS:
			incline_disable();
			rollrotation_disable();
			break;
		case MOTOR_INCLINE:
			incline_disable();
			break;
		case MOTOR_ROLLROTATION:
			rollrotation_disable();
			break;
	}
}

void command_positioning_motor(uint8_t * data, uint8_t length) {
	switch (*data) {
		case ALL_MOTORS:
			//incline_positioning();
			rollrotation_positioning();
			break;
		case MOTOR_INCLINE:
			//incline_positioning();
			break;
		case MOTOR_ROLLROTATION:
			rollrotation_positioning();
			break;
	}
}

void command_save_zero_motor(uint8_t * data, uint8_t length) {
	switch (*data) {
		case ALL_MOTORS:
			incline_save_zero();
			rollrotation_save_zero();
			break;
		case MOTOR_INCLINE:
			incline_save_zero();
			break;
		case MOTOR_ROLLROTATION:
			rollrotation_save_zero();
			break;
	}
}

void command_clear_zero_motor(uint8_t * data, uint8_t length) {
	switch (*data) {
		case ALL_MOTORS:
			incline_zero_reset();
			rollrotation_zero_reset();
			break;
		case MOTOR_INCLINE:
			incline_zero_reset();
			break;
		case MOTOR_ROLLROTATION:
			rollrotation_zero_reset();
			break;
	}
}

void command_incline(uint8_t * data, uint8_t length) {
	incline_set_angle(*data);
}

void command_rollrotation(uint8_t * data, uint8_t length) {
	int16_t pos = 0;
	memcpy(&pos, data, sizeof(uint16_t));	
	rollrotation_goto(pos);
}