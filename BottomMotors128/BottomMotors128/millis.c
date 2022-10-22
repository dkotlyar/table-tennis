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

#include "message.h"

#include "lift.h"
#include "rotation.h"

#include "ultrasonic.h"
#include "infrared.h"

uint32_t _millis = 0;

void millis100(void);
void millis1000(void);

ISR(MILLIS_vect) {
	_millis++;

	//key_handle();
	
	rotation_speed_change();
	lift_change_speed();

	//if (!(_millis % 250)) {
		//ultrasonic_trig();
	//}
	
	if (!(_millis % 100)) {
		millis100();
	}
	if (!(_millis % 1000)) {
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
	OCR0	= ((F_CPU / 64) / MILLIS_TIMER_FREQ) - 1;
	
	TIMSK	|= (1<<OCIE0);
}

void millis100(void) {
	uint8_t rotation_ready = rotation_check_ready();
	uint8_t lift_ready = lift_check_ready();

	#ifdef SHOW_READY_ON_LED
		if (!(rotation_ready & lift_ready)) {
			led_enable();
		} else {
			led_disable();
		}
	#endif

	rotation_led_disable();
	if (rotation_is_min()) {
		rotation_led_min();
	}
	if (rotation_is_max()) {
		rotation_led_max();
	}
	
	lift_led_disable();
	if (lift_is_min()) {
		lift_led_min();
	}
	if (lift_is_max()) {
		lift_led_max();
	}
}

void millis1000(void) {
	//ultrasonic_trig();
	
	//uint16_t adc = infrared_get_adc();
	////uint16_t distance = infrared_get_distance();
////
	//uint8_t data[5];
	//*data = 0;
	//memcpy(data+1,&adc,2);
	////memcpy(data+3,&distance,2);
	//transmit_to1(data,3);
}

uint32_t millis(void) {
	return _millis;
}

uint64_t micros(void) {
	return (uint64_t)_millis * 1000 + TCNT0 * 4;
}