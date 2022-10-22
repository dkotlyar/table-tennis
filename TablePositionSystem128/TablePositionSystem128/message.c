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


void command_led(uint8_t * data, uint8_t length);


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