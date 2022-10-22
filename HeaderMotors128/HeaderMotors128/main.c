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
#include "eeprom.h"

#include "incline.h"
#include "rollrotation.h"

void init(void);

int main(void) {
	cli();
	
	init();
	
	led_enable();
	_delay_ms(1500);
	led_disable();
	
	sei();

	rollrotation_enable();
	incline_enable();

	while (1) {
		usart_handle_receive();
		usart_handle_transmit();
		
	}
}

void init(void) {
	led_init();
	key_init();
	
	usart1_init();
	timer0_init();
	
	incline_init();
	rollrotation_init();
}