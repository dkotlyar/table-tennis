/*
 * millis.c
 *
 * Created: 25.03.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "millis.h"
#include "keys.h"
#include "led.h"
#include "incline.h"
#include "rollrotation.h"

uint32_t millis = 0;

void millis100(void);
void millis1000(void);

ISR(TIMER0_COMP_vect) {
	millis++;
	key_handle();
	rollrotation_change_speed();
	incline_ready_ticks();
	
	if (!(millis % 100)) {
		millis100();
	}
	if (!(millis % 1000)) {
		millis1000();
	}
}

void timer0_init(void) {
	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: 250,000 kHz
	// Mode: CTC top=OCR0
	// OC0 output: Disconnected
	// Prescale: 64
	ASSR	= (0<<AS0);
	TCCR0	= (0<<WGM00) | (1<<WGM01) | 
			  (0<<COM01) | (0<<COM00) | 
			  (1<<CS02) | (0<<CS01) | (0<<CS00);
	TCNT0	= 0x00;
	OCR0	= ((F_CPU / MILLIS_PRESCALE) / MILLIS_TIMER_FREQ) - 1;
	
	TIMSK	|= (1<<OCIE0);
}

void millis100(void) {
	uint8_t incline_ready = incline_check_ready();
	uint8_t rollrotation_ready = rollrotation_check_ready();
	#ifdef SHOW_READY_ON_LED
	if (!(incline_ready & rollrotation_ready)) {
		led_enable();
	} else {
		led_disable();
	}
	#endif
	
	rollrotation_led_disable();
	if (rollrotation_is_min()) {
		rollrotation_led_min();
	}
	if (rollrotation_is_max()) {
		rollrotation_led_max();
	}
	
	incline_led_disable();
	if (incline_is_min()) {
		incline_led_min();
	}
	if (incline_is_max()) {
		incline_led_max();
	}
}

void millis1000(void) {
}