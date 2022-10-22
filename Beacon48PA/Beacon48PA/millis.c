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

ISR(TIMER0_COMPA_vect) {
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
	// Mode: CTC top=OCR0A
	// OC0A output: Disconnected
	// OC0B output: Disconnected
	// Timer Period: 4 us
	TCCR0A	= (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);
	TCCR0B	= (0<<WGM02) | (0<<CS02) | (1<<CS01) | (1<<CS00);
	TCNT0	= 0x00;
	OCR0A	= ((F_CPU / 64) / MILLIS_TIMER_FREQ) - 1;
	OCR0B	= 0;
	
	TIMSK0	= (0<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);

}

uint32_t millis(void) {
	return _millis;
}

uint64_t micros(void) {
	return (uint64_t)_millis * 1000 + TCNT0 * 4;
}