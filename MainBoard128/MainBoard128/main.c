/*
 * main.c
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "usart.h"
#include "millis.h"
#include "keys.h"
#include "led.h"
#include "motor.h"
#include "training.h"

void init(void);

int main(void) {
	cli();
	
	init();
	
	led_enable();
	_delay_ms(1500);
	led_disable();
	
	sei();	
	while (1) {
		usart_handle_receive(get_usart0());
		usart_handle_receive(get_usart1());

		usart_handle_transmit(get_usart0());
		usart_handle_transmit(get_usart1());
	}
}

void init(void) {
	led_init();
	// key_init();
	
	usart0_init();
	usart1_init();
	
	timer0_init();
	
	feed_init();
	training_init();
}