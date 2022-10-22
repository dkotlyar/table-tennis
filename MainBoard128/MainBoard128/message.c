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
#include "motor.h"

#include "training.h"

void command_led(uint8_t * data, uint8_t length);
void command_enable_motor(uint8_t * data, uint8_t length);
void command_disable_motor(uint8_t * data, uint8_t length);

void command_set_mode(uint8_t * data, uint8_t length);
void command_start_program(uint8_t * data, uint8_t length);
void command_stop_program(uint8_t * data, uint8_t length);
void command_pause_program(uint8_t * data, uint8_t length);

void command_trainer_position(uint8_t * data, uint8_t length);

void command_clear_program(uint8_t * data, uint8_t length);
void command_append_shot(uint8_t * data, uint8_t length);
void command_update_shot(uint8_t * data, uint8_t length);
void command_repeat_count(uint8_t * data, uint8_t length);

void add_transmit(uint8_t * data, uint8_t length) {
	transmit_to1(data, length);
} 

void transmit_to0(uint8_t * data, uint8_t length) {
	usart_add_transmit(get_usart0(), data, length);
}

void transmit_to1(uint8_t * data, uint8_t length) {
	usart_add_transmit(get_usart1(), data, length);
}


void handle_message(uint8_t * data, uint8_t length) {
	handle_from1(data, length);
}

void handle_from0(uint8_t * data, uint8_t length) {
	void (*f)(uint8_t*,uint8_t);

	switch (*data) {
		case EMPTY:
			transmit_to1(data, 1);
			return;

		case LED:
			f = command_led;
			transmit_to1(data, length);
			break;

		case SET_MODE:
			f = command_set_mode;
			break;
		case START_PROGRAM:
			f = command_start_program;
			break;
		case STOP_PROGRAM:
			f = command_stop_program;
			break;
		case PAUSE_PROGRAM:
			f = command_pause_program;
			break;

		case TRAINER_POSITION:
			f = command_trainer_position;
			break;

		case CLEAR_PROGRAM:
			f = command_clear_program;
			break;
		case APPEND_SHOT:
			f = command_append_shot;
			break;
		case UPDATE_SHOT:
			f = command_update_shot;
			break;
		case REPEAT_COUNT:
			f = command_repeat_count;
			break;
			
		case ENABLE_MOTOR:
			f = command_enable_motor;
			if (get_mode() == HAND_MODE) {
				transmit_to1(data, length);
			}
			break;
		case DISABLE_MOTOR:
			f = command_disable_motor;
			if (get_mode() == HAND_MODE) {
				transmit_to1(data, length);
			}
			break;
		case POSITIONING_MOTOR:
		case SAVE_ZERO_MOTOR:
		case CLEAR_ZERO_MOTOR:
			transmit_to1(data, length);
			return;

		case LIFT:
		case ROTATION:
		case INCLINE:
		case ROLLROTATION:
		case TR_SPEED:
		case BR_SPEED:
			if (get_mode() == HAND_MODE) {
				transmit_to1(data, length);
			}
			return;
						
		default:
			return;
	}
	
	f(data+1, length-1);
}

void handle_from1(uint8_t * data, uint8_t length) {
	void (*f)(uint8_t*,uint8_t);
		
	switch (*data) {
		
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
		case MOTOR_FEED:
			feed_enable();
			break;
	}
}

void command_disable_motor(uint8_t * data, uint8_t length) {
	switch (*data) {
		case MOTOR_FEED:
			feed_disable();
			break;
	}
}

void command_set_mode(uint8_t * data, uint8_t length) {
	set_mode(*data);
}

void command_start_program(uint8_t * data, uint8_t length) {
	start_program();
}

void command_stop_program(uint8_t * data, uint8_t length) {
	stop_program();
}

void command_pause_program(uint8_t * data, uint8_t length) {
	pause_program();
}

void command_clear_program(uint8_t * data, uint8_t length) {
	clear_program();
}

void command_append_shot(uint8_t * data, uint8_t length) {
	position_t pos;

	memcpy(&pos.lift, data, sizeof(uint16_t));
	data += sizeof(uint16_t);
	
	memcpy(&pos.rotation, data, sizeof(int16_t));
	data += sizeof(int16_t);

	memcpy(&pos.incline, data, sizeof(int8_t));
	data += sizeof(int8_t);

	memcpy(&pos.rollrotation, data, sizeof(int16_t));
	data += sizeof(int16_t);

	memcpy(&pos.toproll, data, sizeof(int16_t));
	data += sizeof(int16_t);

	memcpy(&pos.bottomroll, data, sizeof(int16_t));
	data += sizeof(int16_t);

	memcpy(&pos.delay_ms, data, sizeof(uint8_t));
	data += sizeof(uint8_t);

	append_shot(pos);
}

void command_update_shot(uint8_t * data, uint8_t length) {
	
}

void command_repeat_count(uint8_t * data, uint8_t length) {
	set_repeat_count(*data);
}

void command_trainer_position(uint8_t * data, uint8_t length) {
	uint8_t res[2] = { ERROR, TRAINER_POSITION };
	transmit_to0(res, 2);
}