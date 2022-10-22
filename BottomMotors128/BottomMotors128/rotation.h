/*
 * rotation.h
 *
 * Created: 25.06.2016 10:45:32
 *  Author: Dmitriy
 */ 


#ifndef ROTATION_H_
#define ROTATION_H_

#define ROTATION_DDR		DDRB
#define ROTATION_PIN		5

#define ROTATION_OCR		OCR1A
#define ROTATION_PRESCALE	8

#define ROTATION_ENABLE_PORT	PORTA
#define ROTATION_ENABLE_DDR		DDRA
#define ROTATION_ENABLE_PIN		0

#define ROTATION_DIR_PORT		PORTA
#define ROTATION_DIR_DDR		DDRA
#define ROTATION_DIR_PIN		1

#define ROTATION_INT_PORT		PING
#define ROTATION_INT_DDR		DDRG
#define ROTATION_INT_MIN_PIN	0
#define ROTATION_INT_MAX_PIN	1

#define ROTATION_READY_PORT		PORTC
#define ROTATION_READY_DDR		DDRC
#define ROTATION_READY_PIN		1

#define ROTATION_LED_PORT		PORTC
#define ROTATION_LED_DDR		DDRC
#define ROTATION_LED_MIN_PIN	2
#define ROTATION_LED_MAX_PIN	3

//#define ROTATION_START_FREQ			100
//#define ROTATION_WORK_FREQ			600
//#define ROTATION_FINISH_FREQ		0
//#define ROTATION_POSITIONING_FREQ	400
//
//#define ROTATION_ACCELERATION		50
//#define ROTATION_DECELERATION		10

#define ROTATION_POSITIONING_ANGLE_RANGE 40

#define ROTATION_START_FREQ			200
#define ROTATION_WORK_FREQ			600
#define ROTATION_FINISH_FREQ		400

#define ROTATION_ACCELERATION		2.5
#define ROTATION_DECELERATION		5

#define ROTATION_MINIMUM_POSITION	-120
#define ROTATION_MAXIMUM_POSITION	120

#define ROTATION_MICROSTEP			2
#define ROTATION_GEAR_RATIO			(15.0/45.0)
#define ROTATION_STEPS_PER_ROUND	(200.0 * ROTATION_MICROSTEP)
#define ROTATION_DEGREE_BY_STEP		(360.0 / ROTATION_STEPS_PER_ROUND * ROTATION_GEAR_RATIO)

#define BEACON_PORT		PORTF
#define BEACON_DDR		DDRF
#define BEACON_PIN		PINF
#define BEACON_IR		0
#define BEACON_TRIG		1
#define BEACON_ECHO		2

#define SOUND_CONST 20.047
#define KELVIN_ZERO 273.15
#define TEMPERATURE 22
#define STEP_DELAY_US		50000
#define ULTRASONIC_COUNT	5

void rotation_init(void);
void timer1_init(void);

uint16_t rotation_freq_to_ocr_tick(uint16_t freq);

void rotation_goto(int16_t position);
void rotation_enable_positioning(void);
void rotation_positioning(void);
void rotation_positioning_second(void);

void rotation_enable(void);
void rotation_disable(void);
void rotation_stop(void);
void rotation_start(void);
uint8_t rotation_is_max(void);
uint8_t rotation_is_min(void);
void rotation_set_dir(uint8_t direction);
void rotation_speed_change(void);

void rotation_acceleration_enable(void);
void rotation_acceleration_disable(void);
void rotation_deceleration_enable(void);
void rotation_deceleration_disable(void);

void rotation_led_min(void);
void rotation_led_max(void);
void rotation_led_disable(void);

uint8_t rotation_check_ready(void);

#endif /* ROTATION_H_ */