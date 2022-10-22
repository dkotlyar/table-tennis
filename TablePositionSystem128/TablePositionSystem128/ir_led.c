/*
 * ir_led.c
 *
 * Created: 13.01.2017 11:21:42
 *  Author: Dmitry
 */ 

#include "main.h"
#include "ir_led.h"

ISR (TIMER3_COMPA_vect) {
	IRLED_PORT ^= (1<<IRLED_PIN);
}

void irled_init(void) {
	IRLED_DDR |= (1<<IRLED_PIN);
	IR_EXT_DDR &= ~(1<<IR_EXT_PIN);
	timer2_init();
}

void timer2_init(void) {
	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: 2000,000 kHz
	// Mode: CTC top=OCR2A
	// OC2 output: Disconnected
	// Timer Period: 0,5 us
	//TCCR2	= (0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (0<<CS21) | (0<<CS20);
	//TCNT2	= 0x00;
	//OCR2	= 0x00;
	//
	//TIMSK	|= (1<<OCIE2);

	// Timer/Counter 3 initialization
	// Clock source: System Clock
	// Clock value: 16000,000 kHz
	// Mode: CTC top=ICR3
	// OC3A output: Disconnected
	// OC3B output: Disconnected
	// OC3C output: Disconnected
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer Period: 0,0625 us
	// Timer3 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: On
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off
	TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30);
	TCCR3B=(0<<ICNC3) | (0<<ICES3) | (1<<WGM33) | (1<<WGM32) | (0<<CS32) | (0<<CS31) | (0<<CS30);
	TCNT3H=0x00;
	TCNT3L=0x00;
	ICR3H=0x00;
	ICR3L=0x00;
	OCR3AH=0x00;
	OCR3AL=0x00;
	OCR3BH=0x00;
	OCR3BL=0x00;
	OCR3CH=0x00;
	OCR3CL=0x00;

	ETIMSK=(0<<TICIE3) | (1<<OCIE3A) | (0<<OCIE3B) | (0<<TOIE3) | (0<<OCIE3C) | (0<<OCIE1C);

}

void irled_enable(uint16_t irled_freq) {
	//OCR2 = (F_CPU / 8 / 2 / irled_freq) - 1;
	//TCCR2 |= (0<<CS22) | (1<<CS21) | (0<<CS20);

	ICR3 = (F_CPU / 2 / irled_freq) - 1;
	TCCR3B |= (0<<CS32) | (0<<CS31) | (1<<CS30);
}

void irled_disable(void) {
	TCCR2 &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
	IRLED_PORT &= ~(1<<IRLED_PIN);
}

void irled_check(void) {
	if (IR_EXT_INPUT & (1<<IR_EXT_PIN)) {
		irled_enable(IRLED_FREQ);
	} else {
		irled_disable();
	}
}