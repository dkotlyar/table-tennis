/*
 * training.h
 *
 * Created: 8.08.2016
 * Author: Dmitry Kotlyar
 */ 

#ifndef TRAINING_H_
#define TRAINING_H_

typedef struct __position__ {
	int16_t lift;
	int16_t rotation;
	int8_t incline;
	int16_t rollrotation;

	int16_t toproll;
	int16_t bottomroll;

	uint16_t delay_ms;
} position_t;

enum power_modes {
	HAND_MODE		= 0,
	PROGRAM_MODE	= 1,
	SHOT_MODE		= 2
};

enum program_statuses {
	PROGRAM_STOPPED	= 0,
	PROGRAM_STARTED	= 1,
	PROGRAM_PAUSED	= 2
};

#define STOK_DELAY_MS		100
#define MINIMUM_DELAY_MS	100
#define OVER_DELAY_MS		1000
#define MAXIMUM_DELAY_MS	10000

#define SHOT_LED_PORT		PORTB
#define SHOT_LED_DDR		DDRB
#define SHOT_LED_A			5
#define SHOT_LED_B			6
#define SHOT_LED_C			7

#define CYMOMETR_FREQ		50000UL
#define MAX_CYMOMETR_COUNT	0xAFFF
#define SHOT_SPEED_DISTANCE	10

#define READY_PORT			PORTC
#define READY_DDR			DDRC
#define READY_PIN			PINC
#define READY_LED_PORT		PORTA
#define READY_LED_DDR		DDRA

#define READY_LIFT			0
#define READY_ROTATION		1
#define READY_INCLINE		2
#define READY_ROLLROTATION	3
#define READY_TOPROLL		4
#define READY_BOTTOMROLL	5

void training_init(void);
void timer1_init(void);

void cymemotr_timer_enable(void);
void cymemotr_timer_disable(void);

void set_mode(uint8_t _mode);
uint8_t get_mode(void);
uint8_t check_ready(void);
uint8_t check_full_ready(void);

void disable_collector_motors(void);

void start_program(void);
void stop_program(void);
void pause_program(void);

void next_step(void);
void training_tick(void);

void set_repeat_count(uint8_t repeat_count);
void clear_program(void);
void append_shot(position_t pos);

void set_lift(int16_t lift);
void set_rotation(int16_t rotation);
void set_incline(int8_t incline);
void set_rollrotation(int16_t rollrotation);
void set_top_speed(int16_t toproll);
void set_bottom_speed(int16_t bottomroll);

#endif /* TRAINING_H_ */