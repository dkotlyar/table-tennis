/*
 * ultrasonic.c
 *
 * Created: 19.08.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "led.h"
#include "ultrasonic.h"

#include "millis.h"

uint16_t overflow_ticks, distance;

//ISR (INT5_vect) {
	//if (ULTRASONIC_PIN & (1<<ULTRASONIC_ECHO)) {
		//ultrasonic_enable();
	//} else {
		//uint8_t tcnt = TCNT2;
		//ultrasonic_disable();
//
		//uint32_t timer_ticks = ((overflow_ticks << 8) | tcnt) + 1;
		//distance = (uint16_t)(timer_ticks / (F_CPU / 1000000UL) * ULTRASONIC_PRESCALE) / ULTRASONIC_CM_CONST;
			//
		////uint8_t data[3];
		////*data = 0;
		////memcpy(data+1, &distance, 2);
		////transmit_to1(data, 3);
	//}
//}

//ISR (TIMER2_OVF_vect) {
	//overflow_ticks++;
//}

void ultrasonic_init(void) {
	ULTRASONIC_DDR |= (1<<ULTRASONIC_TRIG);
	ULTRASONIC_DDR &= ~(1<<ULTRASONIC_ECHO);

	//EICRB	|= (0<<ISC51) | (1<<ISC50);
	//EIMSK	|= (1<<INT5);
	//EIFR	|= (1<<INTF5);
//
	//timer2_init();
}

void timer2_init(void) {
	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: 2000,000 kHz
	// Mode: Normal top=0xFF
	// OC2 output: Disconnected
	// Timer Period: 0,128 ms
	TCCR2	=	(0<<WGM20) | (0<<WGM21) |
				(0<<COM21) | (0<<COM20) |
				(0<<CS22) | (0<<CS21) | (0<<CS20);
	TCNT2	= 0;
	OCR2	= 0;

	TIMSK	|= (1<<TOIE2);
}

void ultrasonic_enable(void) {
	overflow_ticks = 0;
	TCNT2 = 0;

	#if ULTRASONIC_PRESCALE == 1
	TCCR2 |= (0<<CS22) | (0<<CS21) | (1<<CS20);
	#elif ULTRASONIC_PRESCALE == 8
	TCCR2 |= (0<<CS22) | (1<<CS21) | (0<<CS20);
	#elif ULTRASONIC_PRESCALE == 64
	TCCR2 |= (0<<CS22) | (1<<CS21) | (1<<CS20);
	#elif ULTRASONIC_PRESCALE == 256
	TCCR2 |= (1<<CS22) | (0<<CS21) | (0<<CS20);
	#elif ULTRASONIC_PRESCALE == 1024
	TCCR2 |= (1<<CS22) | (0<<CS21) | (1<<CS20);
	#else
	#error ULTRASONIC PRESCALE NOT MATCH
	#endif
}

void ultrasonic_disable(void) {
	TCCR2 &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
}

void ultrasonic_trig(void) {
	ULTRASONIC_PORT |= (1<<ULTRASONIC_TRIG);
	_delay_us(10);
	ULTRASONIC_PORT &= ~(1<<ULTRASONIC_TRIG);
}

uint16_t ultrasonic_get_distance(void) {
	//ultrasonic_trig();
//
	//uint16_t delay = 2000;
	//distance = 0;
//
	//while (!distance && delay) { // ожидание 0.1 с 
		//_delay_us(50); 
		//delay--;
	//}
//
	//if (delay)
		//return distance;
	//else
		//return 0xFFFF;

	//return distance;

	//#define DELAY_STEP 10
//
	//uint16_t delay_us = 1000000;
	//delay_us /= DELAY_STEP;

	ULTRASONIC_PORT |= (1<<ULTRASONIC_TRIG);
	_delay_us(10);
	ULTRASONIC_PORT &= ~(1<<ULTRASONIC_TRIG);

	while (!(ULTRASONIC_PIN & (1<<ULTRASONIC_ECHO))) ;

	uint64_t time_start = micros();

	while ((ULTRASONIC_PIN & (1<<ULTRASONIC_ECHO))) ;

	uint64_t time_end = micros();

	return (time_end - time_start) * 10 / ULTRASONIC_CM_CONST;
}