/*
 * incline.h
 *
 * Created: 07.06.2016 13:50:05
 *  Author: Dmitriy
 */ 


#ifndef INCLINE_H_
#define INCLINE_H_

#define INCLINE_DDR	DDRB
#define INCLINE_PIN	5
#define INCLINE_OCR OCR1A

#define INCLINE_READY_PORT	PORTC
#define INCLINE_READY_DDR	DDRC
#define INCLINE_READY_PIN	0

#define INCLINE_ENABLE_PORT PORTG
#define INCLINE_ENABLE_DDR DDRG
#define INCLINE_ENABLE_PIN 0

#define INCLINE_INT_DDR		DDRA
#define INCLINE_INT_PORT	PINA
#define INCLINE_INT_MIN_PIN	6
#define INCLINE_INT_MAX_PIN	7

#define INCLINE_LED_PORT	PORTC
#define INCLINE_LED_DDR		DDRC
#define INCLINE_LED_MIN_PIN	4
#define INCLINE_LED_MAX_PIN	5

// 50 Hz = 20 ms
#define SERVO_FREQ	50 // Ãö
#define SERVO_PWM	((F_CPU / 8) / SERVO_FREQ - 1)

#define SERVO_ANGEL 90

#define SERVO_MIN 1000 // us
#define SERVO_MAX 2000 // us
#define SERVO_ZERO ((SERVO_MAX - SERVO_MIN) / 2 + SERVO_MIN)

#define SERVO_US_PER_ANGLE ((SERVO_MAX - SERVO_MIN) / SERVO_ANGEL)

#define SERVO_TRAVEL_TIME_ms 500

void incline_init(void);
void timer1_init(void);

uint16_t angle_to_pwm(int8_t __angle);

void incline_enable(void);
void incline_disable(void);

void incline_set_angle(int8_t __angle);
int8_t incline_get_angle(void);
void incline_save_zero(void);
void incline_zero_reset(void);

void incline_ready_ticks(void);
uint8_t incline_check_ready(void);

uint8_t incline_is_max(void);
uint8_t incline_is_min(void);

void incline_led_min(void);
void incline_led_max(void);
void incline_led_disable(void);

#endif /* INCLINE_H_ */