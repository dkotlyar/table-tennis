/*
 * lift.h
 *
 * Created: 24.06.2016 20:56:39
 *  Author: Dmitriy
 */ 


#ifndef LIFT_H_
#define LIFT_H_

#define LIFT_DDR			DDRE
#define LIFT_PIN			3

#define LIFT_OCR			OCR3A
#define LIFT_PRESCALE		8

#define LIFT_ENABLE_PORT	PORTA
#define LIFT_ENABLE_DDR		DDRA
#define LIFT_ENABLE_PIN		2

#define LIFT_DIR_PORT		PORTA
#define LIFT_DIR_DDR		DDRA
#define LIFT_DIR_PIN		3

#define LIFT_INT_PORT		PING
#define LIFT_INT_DDR		DDRG
#define LIFT_INT_MAX_PIN	4
#define LIFT_INT_MIN_PIN	3

#define LIFT_READY_PORT		PORTC
#define LIFT_READY_DDR		DDRC
#define LIFT_READY_PIN		0

#define LIFT_LED_PORT		PORTC
#define LIFT_LED_DDR		DDRC
#define LIFT_LED_MIN_PIN	4
#define LIFT_LED_MAX_PIN	5

//#define LIFT_START_FREQ			1600
//#define LIFT_WORK_FREQ			2200
//#define LIFT_FINISH_FREQ		1000
#define LIFT_START_FREQ			1200
#define LIFT_WORK_FREQ			1400
#define LIFT_FINISH_FREQ		1000

//#define LIFT_ACCELERATION		0.15
//#define LIFT_DECELERATION		0.75
#define LIFT_ACCELERATION		0.25
#define LIFT_DECELERATION		0.75

#define LIFT_MAXIMUM_POSITION	1390
#define LIFT_MINIMUM_POSITION	553
#define LIFT_DISTANCE_PER_STEP	0.101665
	// (PI * Z-sheave * Drive-sheave / Engine-sheave / Clock-cycle)
	// Z-sheave = 19.5 (малый шкив на штанге) (19.41658474732504717313228136392)
	// Drive-sheave = 20 (шкив на двигателе)
	// Engine-sheave = 30 (шкив на валу)
	// Сlock-cycle = 400 (шагов на оборот двигателя)

void lift_init(void);
void timer3_init(void);
uint16_t lift_freq_to_ocr_tick(uint16_t freq);
void lift_goto(int16_t position);
void lift_positioning(void);
void lift_enable(void);
void lift_disable(void);
void lift_stop(void);
void lift_start(void);
uint8_t lift_is_max(void);
uint8_t lift_is_min(void);
void lift_set_dir(uint8_t direction);
void lift_change_speed(void);

void lift_acceleration_enable(void);
void lift_acceleration_disable(void);
void lift_deceleration_enable(void);
void lift_deceleration_disable(void);

void lift_led_min(void);
void lift_led_max(void);
void lift_led_disable(void);

uint8_t lift_check_ready(void);
uint8_t lift_is_travel(void);

#endif /* LIFT_H_ */