/*
 * rollrot.h
 *
 * Created: 21.06.2016 11:26:45
 * Author: Dmitriy
 */ 


#ifndef ROLLROT_H_
#define ROLLROT_H_

#define ROLLROTATION_MICROSTEP			1.0
#define ROLLROTATION_GEAR_RATIO			(8.0/56.0)
#define ROLLROTATION_STEPS_PER_ROUND	(200.0 * ROLLROTATION_MICROSTEP)
#define ROLLROTATION_DEGREE_BY_STEP		(360.0 / ROLLROTATION_STEPS_PER_ROUND * ROLLROTATION_GEAR_RATIO)

//#define ROLLROTATION_START_FREQ		200
//#define ROLLROTATION_WORK_FREQ		500
//#define ROLLROTATION_FINISH_FREQ	150
//
////#define ROLLROTATION_ACCELERATION		0.1
//#define ROLLROTATION_ACCELERATION		2
//#define ROLLROTATION_DECELERATION		0.5

#define ROLLROTATION_START_FREQ		300
#define ROLLROTATION_WORK_FREQ		600
#define ROLLROTATION_FINISH_FREQ	600

#define ROLLROTATION_ACCELERATION		0.1
//#define ROLLROTATION_DECELERATION		0.0000001

#define ROLLROTATION_PRESCALE	1024
#define ROLLROTATION_OCR		OCR2

#define ROLLROTATION_EN_PORT	PORTA
#define ROLLROTATION_EN_DDR		DDRA
#define ROLLROTATION_EN_PIN		0

#define ROLLROTATION_DDR	DDRB
#define ROLLROTATION_PIN	7
#define ROLLROTATION_OCR	OCR2

#define ROLLROTATION_DIR_PORT	PORTA
#define ROLLROTATION_DIR_DDR	DDRA
#define ROLLROTATION_DIR_PIN	1

#define ROLLROTATION_ANGLE_MIN	-120
#define ROLLROTATION_ANGLE_MAX	120

#define ROLLROTATION_READY_PORT	PORTC
#define ROLLROTATION_READY_DDR	DDRC
#define ROLLROTATION_READY_PIN	1

#define ROLLROTATION_INT_DDR		DDRA
#define ROLLROTATION_INT_PORT		PINA
#define ROLLROTATION_INT_MIN_PIN	5
#define ROLLROTATION_INT_MAX_PIN	4

#define ROLLROTATION_LED_PORT		PORTC
#define ROLLROTATION_LED_DDR		DDRC
#define ROLLROTATION_LED_MIN_PIN	2
#define ROLLROTATION_LED_MAX_PIN	3

void rollrotation_init(void);
void timer2_init(void);

void rollrotation_save_zero(void);
void rollrotation_zero_reset(void);

void rollrotation_enable(void);
void rollrotation_disable(void);
uint8_t rollrotation_freq_to_ocr_ticks(uint16_t freq);

void rollrotation_positioning(void);
void rollrotation_goto(int16_t position);
void rollrotation_set_dir(uint8_t direction);
void rollrotation_start(void);
void rollrotation_stop(void);
void rollrotation_change_speed(void);

uint8_t rollrotation_is_max(void);
uint8_t rollrotation_is_min(void);

void rollrotation_acceleration_enable(void);
void rollrotation_acceleration_disable(void);
void rollrotation_deceleration_enable(void);
void rollrotation_deceleration_disable(void);

void rollrotation_led_min(void);
void rollrotation_led_max(void);
void rollrotation_led_disable(void);

uint8_t rollrotation_check_ready(void);

//uint8_t rollrotation_start_speed(void);
//void rollrotation_save_start_speed(uint8_t speed);
//uint8_t rollrotation_work_speed(void);
//void rollrotation_save_work_speed(uint8_t speed);


#endif /* ROLLROT_H_ */