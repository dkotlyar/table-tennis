/*
 * TablePositionSystem128.c
 *
 * Created: 19.01.2017 10:52:44
 * Author : Dmitry
 */ 

#include "main.h"
#include "led.h"
#include "millis.h"
#include "usart.h"
#include "ir_led.h"
#include "ultrasound_receiver.h"
#include "tsop.h"

void init(void);

int main(void)
{
	cli();
	init();

	led_enable();
	_delay_ms(1500);
	led_disable();

	//irled_enable(36000);
	
	sei();

    while (1)
    {
	    //usart_handle_receive(get_usart1());
	    //usart_handle_transmit(get_usart1());

		ultrasound_check();
		irled_check();
		tsop_is_active();
		
		//irled_disable();
		//if (tsop_is_active()) {
			//irled_enable(56000);
			//_delay_us(50);
		//}
    }
}

void init(void) {
	led_init();
	millis_init();
	usart1_init();
	irled_init();
	ultrasound_init();
	tsop_init();
}

