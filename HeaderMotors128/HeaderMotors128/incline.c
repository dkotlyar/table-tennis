/*
 * servo.c
 *
 * Created: 07.06.2016 13:49:55
 *  Author: Dmitriy
 */ 

#include "main.h"
#include "incline.h"
#include "eeprom.h"

struct __incline__ {
	uint8_t enabled;
	int8_t correction;
	int8_t angle;

	uint8_t ready;
	uint16_t ready_ticks;
} incline;

void incline_init(void) {
	incline.correction = eeprom_read(INCLINE_CORRECTION_ADDR);
	
	INCLINE_DDR			|= (1<<INCLINE_PIN);
	INCLINE_READY_DDR	|= (1<<INCLINE_READY_PIN);
	INCLINE_ENABLE_DDR	|= (1<<INCLINE_ENABLE_PIN);
	INCLINE_LED_DDR		|= ((1<<INCLINE_LED_MIN_PIN) | (1<<INCLINE_LED_MAX_PIN));
	INCLINE_INT_DDR		&= ~((1<<INCLINE_INT_MIN_PIN) | (1<<INCLINE_INT_MAX_PIN));
	
	timer1_init();
	incline_set_angle(0);
	incline_disable();
}

void timer1_init(void) {
	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Mode: Fast PWM top=ICR1
	// OC1A output: Non-Inverted PWM
	// OC1B output: Disconnected
	// OC1C output: Disconnected
	// Prescale: 8
	TCCR1A	= (1<<COM1A1) | (0<<COM1A0) | 
			(0<<COM1B1) | (0<<COM1B0) | 
			(0<<COM1C1) | (0<<COM1C0) | 
			(1<<WGM11) | (0<<WGM10);
	TCCR1B	= (0<<ICNC1) | (0<<ICES1) | 
			(1<<WGM13) | (1<<WGM12) | 
			(0<<CS12) | (1<<CS11) | (0<<CS10);
	
	TCNT1	= 0;
	ICR1	= SERVO_PWM;
	OCR1A	= 0;
	OCR1B	= 0;
	OCR1C	= 0;
}

uint16_t angle_to_pwm(int8_t __angle) {
	uint16_t servo_us = ((SERVO_US_PER_ANGLE * __angle) + SERVO_ZERO);
	if (servo_us > SERVO_MAX) {
		servo_us = SERVO_MAX;
	} else if (servo_us < SERVO_MIN) {
		servo_us = SERVO_MIN;
	}
	return (uint16_t)((F_CPU / 1000000UL * servo_us / 8) - 1);
}

void incline_enable(void) {
	INCLINE_ENABLE_PORT |= (1<<INCLINE_ENABLE_PIN);
	incline.enabled = 1;
	incline.ready = 0;
}

void incline_disable(void) {
	INCLINE_ENABLE_PORT &= ~(1<<INCLINE_ENABLE_PIN);
	incline.enabled = 0;
	incline.ready = 0;
}

void incline_set_angle(int8_t __angle) {
	incline.angle = __angle;
	incline.ready = 0;
	incline.ready_ticks = 0;
	
	INCLINE_OCR = angle_to_pwm(incline.angle + incline.correction);
}

int8_t incline_get_angle(void) {
	return incline.angle;
}

void incline_save_zero(void) {
	incline.correction = incline.correction + incline.angle;
	incline.angle = 0;
	eeprom_write(INCLINE_CORRECTION_ADDR, incline.correction);
}

void incline_zero_reset(void) {
	incline.correction = 0;
	eeprom_write(INCLINE_CORRECTION_ADDR, 0);
	incline_set_angle(0);
}

void incline_ready_ticks(void) {
	if (!incline.enabled)
		return;
	
	incline.ready_ticks++;
	if (incline.ready_ticks > SERVO_TRAVEL_TIME_ms) {
		incline.ready = 1;
	}
}

uint8_t incline_check_ready(void) {
	if (!incline.ready) {
		INCLINE_READY_PORT |= (1<<INCLINE_READY_PIN);
	} else {
		INCLINE_READY_PORT &= ~(1<<INCLINE_READY_PIN);
	}
	return incline.ready;
}

uint8_t incline_is_max(void) {
	return ((INCLINE_INT_PORT & (1<<INCLINE_INT_MAX_PIN)) == 0);
}

uint8_t incline_is_min(void) {
	return ((INCLINE_INT_PORT & (1<<INCLINE_INT_MIN_PIN)) == 0);
}


void incline_led_min(void) {
	INCLINE_LED_PORT |= (1<<INCLINE_LED_MIN_PIN);
}

void incline_led_max(void) {
	INCLINE_LED_PORT |= (1<<INCLINE_LED_MAX_PIN);
}

void incline_led_disable(void) {
	INCLINE_LED_PORT &= ~((1<<INCLINE_LED_MIN_PIN) | (1<<INCLINE_LED_MAX_PIN));
}