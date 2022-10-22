/*
 * main.c
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "usart.h"
#include "millis.h"
#include "led.h"
#include "motors.h"

void init(void);
void led_init(void);

uint8_t need_send_speed = 0;

int main(void) {
	cli();
	
	init();
	
	led_enable();
	_delay_ms(1500);
	led_disable();
	
	sei();
	while (1) {
		usart_handle_receive();
		usart_handle_transmit();
		regulate_all_motors();
		
		if (need_send_speed) {
			motors_send_speed();
			need_send_speed = 0;
		}
	}
}

void init(void) {	
	led_init();
	usart1_init();
	motors_init();
	timer0_init();
}

void send_speed(void) {
	#ifdef SEND_SPEED
	need_send_speed = 1;
	#endif
}