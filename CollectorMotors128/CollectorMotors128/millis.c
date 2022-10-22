/*
 * millis.c
 *
 * Created: 25.03.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "millis.h"
#include "motors.h"

uint32_t millis = 0;

void millis100(void);
void millis1000(void);

ISR(TIMER0_COMP_vect) {
	millis++;
	
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
	ASSR	= 0<<AS0;
	TCCR0	= (0<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
	TCNT0	= 0x00;
	OCR0	= ((F_CPU / MILLIS_TIMER_PRESCALE) / MILLIS_TIMER_FREQ) - 1;
	
	TIMSK	|= (1<<OCIE0);
}

void millis100(void) {
	motors_check_ready();
	
	static uint8_t send_speed_timeout = 0;
	if ((send_speed_timeout++) % 5 == 0) {
		send_speed();
		send_speed_timeout = 0;
	}
}

void millis1000(void) {
}