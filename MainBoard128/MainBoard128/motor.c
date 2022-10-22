/*
 * motor.c
 *
 * Created: 27.06.2016 13:02:45
 *  Author: Dmitriy
 */ 

#include "main.h"
#include "motor.h"

void feed_init(void) {
	FEED_DDR |= (1<<FEED_PIN);
	
	#ifdef FEED_PWM_ENABLE
	timer3_init();
	#endif
	
	feed_disable();
}

void timer3_init(void) {
	uint16_t ocr = feed_freq_to_ocr_ticks(FEED_FREQ);
	
	// Timer/Counter 3 initialization
	// Clock source: System Clock
	// Clock value: 250,000 kHz
	// Mode: Fast PWM top=ICR3
	// OC3A output: Non-Inverted PWM
	// OC3B output: Disconnected
	// OC3C output: Disconnected
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer Period: 4 us
	// Output Pulse(s):
	// OC3A Period: 4 us
	// Timer3 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off
	TCCR3A	= (1<<COM3A1) | (0<<COM3A0) | 
			  (0<<COM3B1) | (0<<COM3B0) | 
			  (0<<COM3C1) | (0<<COM3C0) | 
			  (1<<WGM31) | (0<<WGM30);
	TCCR3B	= (0<<ICNC3) | (0<<ICES3) | 
			  (1<<WGM33) | (1<<WGM32) | 
			  (0<<CS32) | (1<<CS31) | (1<<CS30);
	TCNT3	= 0x00;
	ICR3	= ocr;
	OCR3A	= ocr * FEED_PWM;
	OCR3B	= 0x00;
	OCR3C	= 0x00;
}

uint16_t feed_freq_to_ocr_ticks(uint16_t freq) {
	return (uint16_t)(F_CPU / FEED_PRESCALE / freq) - 1;
}

void feed_enable(void) {
	#ifdef FEED_PWM_ENABLE
		TCCR3A |= (1<<COM3A1) | (0<<COM3A0);
	#else
		PORTE |= (1<<FEED_PIN);
	#endif
}

void feed_disable(void) {
	#ifdef FEED_PWM_ENABLE
		TCCR3A &= ~((1<<COM3A1) | (1<<COM3A0));
	#else
		PORTE &= ~(1<<FEED_PIN);
	#endif
}