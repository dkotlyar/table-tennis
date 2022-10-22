/*
 * BottomMotors128.c
 *
 * Created: 24.06.2016 14:18:22
 * Author : Dmitriy
 */ 

#include "main.h"
#include "usart.h"

#include "millis.h"
#include "keys.h"
#include "led.h"

#include "rotation.h"
#include "lift.h"

#include "ultrasonic.h"
#include "infrared.h"

void init(void);

int main(void)
{
	cli();
	
	init();
	
	led_enable();
	_delay_ms(1500);
	led_disable();
	
	sei();
	
	lift_enable();
	rotation_enable();

    while (1) 
    {
	    usart_handle_receive(get_usart1());
	    usart_handle_transmit(get_usart1());
		
		rotation_positioning_second();
    }
}

void init(void) {
	led_init();
	//key_init();
	
	usart1_init();
	
	timer0_init();
	
	rotation_init();
	lift_init();

	ultrasonic_init();
	//infrared_init();
}
