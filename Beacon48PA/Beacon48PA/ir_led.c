/*
 * ir_led.c
 *
 * Created: 13.01.2017 11:21:42
 *  Author: Dmitry
 */ 

#include "main.h"
#include "ir_led.h"

ISR (TIMER2_COMPA_vect) {
	IRLED_PORT ^= (1<<IRLED_PIN);
}

void irled_init(void) {
	IRLED_DDR |= (1<<IRLED_PIN);
	timer2_init();
}

void timer2_init(void) {
	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: 16000,000 kHz
	// Mode: CTC top=OCR2A
	// OC2A output: Toggle on compare match
	// OC2B output: Disconnected
	// Timer Period: 0,0625 us
	// Output Pulse(s):
	// OC2A Period: 0,125 us Width: 0,0625 us
	ASSR	= (0<<EXCLK) | (0<<AS2);
	TCCR2A	= (0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
	TCCR2B	= (0<<WGM22) | (0<<CS22) | (0<<CS21) | (0<<CS20);
	TCNT2	= 0x00;
	OCR2A	= (F_CPU / 8 / 2 / IRLED_FREQ);
	OCR2B	= 0x00;

	TIMSK2	= (0<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);
}

void irled_enable(void) {
	//IRLED_PORT |= (1<<IRLED_PIN);
	TCCR2B |= (0<<CS22) | (1<<CS21) | (0<<CS20);
}

void irled_disable(void) {
	TCCR2B &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
	IRLED_PORT &= ~(1<<IRLED_PIN);
}