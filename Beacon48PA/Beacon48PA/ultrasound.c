/*
 * ultrasound.c
 *
 * Created: 13.01.2017 11:26:21
 *  Author: Dmitry
 */ 

#include "main.h"
#include "ultrasound.h"

void us_init(void) {
	US_EN_DDR |= (1<<US_EN_PIN);
	US_SIG_DDR |= ((1<<US_SIG_PIN1) | (1<<US_SIG_PIN2));
	US_LED_DDR |= (1<<US_LED_PIN);
	timer1_init();
}

void timer1_init(void) {
	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 16000,000 kHz
	// Mode: Fast PWM top=ICR1
	// OC1A output: Non-Inverted PWM
	// OC1B output: Inverted PWM
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer Period: 0,0625 us
	// Output Pulse(s):
	// OC1A Period: 0,0625 us
	// OC1B Period: 0,0625 us
	// Timer1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	TCCR1A	= (1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (1<<COM1B0) | (1<<WGM11) | (0<<WGM10);
	TCCR1B	= (0<<ICNC1) | (0<<ICES1) | (1<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
	TCNT1	= 0x00;
	ICR1	= (F_CPU / US_FREQ) - 1;
	OCR1A	= ((F_CPU / US_FREQ) - 1) / 2;
	OCR1B	= ((F_CPU / US_FREQ) - 1) / 2;
}

void us_enable(void) {
	TCCR1B |= ((0<<CS12) | (0<<CS11) | (1<<CS10));
	US_LED_PORT |= (1<<US_LED_PIN);
}

void us_disable(void) {
	TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
	US_LED_PORT &= ~(1<<US_LED_PIN);
}

void us_trig(uint16_t delay_us) {
	us_enable();
	for (uint16_t i = 0; i < delay_us; i++) {
		_delay_us(1);
	}
	us_disable();
}

void us_trig_default(void) {
	us_enable();
	_delay_ms(100);
	us_disable();
}