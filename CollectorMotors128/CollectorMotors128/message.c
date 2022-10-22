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
#include "motors.h"
#include "usart.h"

#include <string.h>

void led(uint8_t * data, uint8_t length);

void tr_width(uint8_t * data, uint8_t length);
void tr_speed(uint8_t * data, uint8_t length);
void tr_stop(uint8_t * data, uint8_t length);
void tr_disable(uint8_t * data, uint8_t length);

void br_width(uint8_t * data, uint8_t length);
void br_speed(uint8_t * data, uint8_t length);
void br_stop(uint8_t * data, uint8_t length);
void br_disable(uint8_t * data, uint8_t length);

void add_transmit(uint8_t * data, uint8_t length) {
	usart_add_transmit(data, length);
} 

void handle_message(uint8_t * data, uint8_t length) {
	void (*f)(uint8_t*,uint8_t);
		
	switch (*data) {
		case LED:
			f = led;
			break;
		
		case TR_WIDTH:
			f = tr_width;
			break;
		case TR_SPEED:
			f = tr_speed;
			break;
		case TR_STOP:
			f = tr_stop;
			break;
		case TR_DISABLE:
			f = tr_disable;
			break;
			
		case BR_WIDTH:
			f = br_width;
			break;
		case BR_SPEED:
			f = br_speed;
			break;
		case BR_STOP:
			f = br_stop;
			break;
		case BR_DISABLE:
			f = br_disable;
			break;
		default:
			return;
	}
	
	f(data+1, length-1);
}

void led(uint8_t * data, uint8_t length) {
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

void tr_width(uint8_t * data, uint8_t length) {
	motor_set_relative_speed(get_motor_top(), *data);
}

void tr_speed(uint8_t * data, uint8_t length) {
	int16_t speed = 0;
	memcpy(&speed, data, sizeof(int16_t));
	motor_set_absolute_speed(get_motor_top(), speed);
}

void tr_stop(uint8_t * data, uint8_t length) {
	motor_stop(get_motor_top());
}

void tr_disable(uint8_t * data, uint8_t length) {
	motor_disable(get_motor_top());
}

void br_width(uint8_t * data, uint8_t length) {
	motor_set_relative_speed(get_motor_bottom(), *data);
}

void br_speed(uint8_t * data, uint8_t length) {
	int16_t speed = 0;
	memcpy(&speed, data, sizeof(int16_t));
	motor_set_absolute_speed(get_motor_bottom(), speed);
}

void br_stop(uint8_t * data, uint8_t length) {
	motor_stop(get_motor_bottom());
}

void br_disable(uint8_t * data, uint8_t length) {
	motor_disable(get_motor_bottom());
}