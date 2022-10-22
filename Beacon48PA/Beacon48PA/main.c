/*
 * Beacon48PA.c
 *
 * Created: 13.01.2017 10:51:49
 * Author : Dmitry
 */ 

#include "main.h"
#include "led.h"
#include "millis.h"
#include "tsop.h"
#include "ir_led.h"
#include "ultrasound.h"

void init(void);

uint8_t transmitted = 0;

int main(void) {
	cli();
	init();
	tsop_enable();
	irled_enable();

	led_enable();
	_delay_ms(1500);
	led_disable();

	sei();

    while (1) 
    {
		if (tsop_is_active()) {
			if (!transmitted) {
				transmitted = 1;
				us_trig_default();
				_delay_ms(500);
			}
		} else {
			transmitted = 0;
		}
    }
}

void init(void) {
	millis_init();
	led_init();
	tsop_init();
	irled_init();
	us_init();
}
