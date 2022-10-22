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

#include "rotation.h"
#include "lift.h"


void command_led(uint8_t * data, uint8_t length);
void command_enable_motor(uint8_t * data, uint8_t length);
void command_disable_motor(uint8_t * data, uint8_t length);
void command_lift(uint8_t * data, uint8_t length);
void command_rotation(uint8_t * data, uint8_t length);
void command_positioning(uint8_t * data, uint8_t length);


void add_transmit(uint8_t * data, uint8_t length) {
	transmit_to1(data, length);
} 

void transmit_to0(uint8_t * data, uint8_t length) {
	usart_add_transmit(get_usart0(), data, length);
}

void transmit_to1(uint8_t * data, uint8_t length) {
	usart_add_transmit(get_usart1(), data, length);
}


void handle_from0(uint8_t * data, uint8_t length) {
}

void handle_from1(uint8_t * data, uint8_t length) {
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
			
		case LIFT:
			f = command_lift;
			break;
		case ROTATION:
			f = command_rotation;
			break;
			
		case POSITIONING_MOTOR:
			f = command_positioning;
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
			led_disable();
			break;
	}
}

void command_enable_motor(uint8_t * data, uint8_t length) {
	switch (*data) {
		case ALL_MOTORS:
			lift_enable();
			rotation_enable();
			break;
		case MOTOR_LIFT:
			lift_enable();
			break;
		case MOTOR_ROTATION:
			rotation_enable();
			break;
	}
}

void command_disable_motor(uint8_t * data, uint8_t length) {
	switch (*data) {
		case ALL_MOTORS:
			lift_disable();
			rotation_disable();
			break;
		case MOTOR_LIFT:
			lift_disable();
			break;
		case MOTOR_ROTATION:
			rotation_disable();
			break;
	}
}

void command_lift(uint8_t * data, uint8_t length) {
	int16_t pos = 0;
	memcpy(&pos, data, sizeof(int16_t));
	lift_goto(pos);
}

void command_rotation(uint8_t * data, uint8_t length) {
	int16_t pos = 0;
	memcpy(&pos, data, sizeof(int16_t));
	rotation_goto(pos);
}

void command_positioning(uint8_t * data, uint8_t length) {
	switch (*data) {
		case ALL_MOTORS:
			lift_positioning();
			rotation_positioning();
			break;
		case MOTOR_LIFT:
			lift_positioning();
			break;
		case MOTOR_ROTATION:
			rotation_enable_positioning();
			break;
	}
}