/*
 * millis.c
 *
 * Created: 25.03.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "millis.h"

#include "tsop.h"

uint32_t _millis = 0;

ISR(MILLIS_vect) {
	_millis++;

	//if (!(_millis % 100)) {
		//tsop_reset();
	//}
}

void millis_init(void) {
	timer0_init();
}

void timer0_init(void) {
	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: 250,000 kHz
	// Mode: CTC top=OCR0
	// OC0 output: Disconnected
	// Prescale: 64
	ASSR	= 0<<AS0;
	TCCR0	= (0<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
	TCNT0	= 0x00;
	OCR0	= ((F_CPU / 64) / MILLIS_TIMER_FREQ) - 1;
	
	TIMSK	|= (1<<OCIE0);
}

uint32_t millis(void) {
	return _millis;
}

uint64_t micros(void) {
	return (uint64_t)_millis * 1000 + TCNT0 * 4;
}