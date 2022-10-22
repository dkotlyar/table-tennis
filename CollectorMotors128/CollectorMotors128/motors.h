/*
 * motors.h
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef MOTORS_H_
#define MOTORS_H_

// Настройки ПИД регулятора
// Быстрый, но не стабильный. 
#define PROPORTIONAL_ADJUSTMENT_FACTOR	0.25
//#define INTEGRATED_ADJUSTMENT_FACTOR	0.0
#define DIFFERENTIAL_ADJUSTMENT_FACTOR	2.0

// Частотомер
#define CYMOMETR_FREQ		50000UL
#define MAX_CYMOMETR_COUNT	0xFFFF
#define CYMOMETR_TIMER_vect	TIMER3_COMPA_vect
#define HALL_TOP_vect		INT1_vect
#define HALL_BOTTOM_vect	INT0_vect

// Значения скорости об/мин
#define MOTOR_MAX_SPEED 10800
#define SPEED_EPSILON 1*60 // отклонение 1 об/мин

// Выход ШИМ
#define PWM_FREQ 2000
#define PWM_TIMER_TOP ((F_CPU / PWM_FREQ) - 1)

#define PWM_DDR DDRB
#define PWM_TOP_PIN 5
#define PWM_BOT_PIN 6

// Сигнал готовности
#define SIG_PORT	PORTC
#define SIG_DDR		DDRC
#define SIG_TOP_PIN	0
#define SIG_BOT_PIN	1

#define OCR_TOP		OCR1A
#define OCR_BOTTOM	OCR1B

#define MODE_DDR		DDRA
#define MODE_PORT		PORTA
#define MODE_TOP_PIN	0
#define MODE_TOP_PIN2	1
#define MODE_BOTTOM_PIN	 2
#define MODE_BOTTOM_PIN2 3

enum motor_status {
	MOTOR_DISABLE	= 0b00,
	ROTATE_FORWARD	= 0b01,
	ROTATE_BACKWARD	= 0b10,
	MOTOR_STOP		= 0b11
};

typedef struct __motor__ {
	uint16_t pwm_ticks;	// Скорость в условных еденицах
	
	float last_integr;	// Предыдущая интеграционная составляющая
	float last_eps;		// Предыдущая ошибка
	
	uint8_t status;
	uint8_t ready;
	uint16_t absolute_speed;	// Установленная скорость, об/мин (от 0 до MAX)
	uint8_t relative_speed;
	
	uint16_t temp_period_ticks;
	uint16_t period_ticks;
	uint8_t  cymometr_status;
	uint16_t actual_speed;		// Реальная скорость, об/мин (от 0 до MAX)

	volatile uint16_t * OCR;
	uint8_t MODE_PIN;
} motor_t;

typedef struct __motors__ {
	motor_t top;
	motor_t bottom;
} motors_t;

void motors_init(void);
void timer1_init(void);
void timer3_init(void);
void cymometr_init(void);

void motor_period_tick(motor_t * motor);
void motor_save_period(motor_t * motor);
void regulate_all_motors(void);
void motor_regulator(motor_t * motor);
void motor_speed_analyze(motor_t * motor);
uint16_t speed_regulator(motor_t * motor);

void motors_check_ready(void);

uint16_t speed_to_pwm(uint16_t speed);
uint16_t width_to_pwm(uint8_t width);

void motor_set_absolute_speed(motor_t * motor, int16_t absolute_speed);
void motor_set_relative_speed(motor_t * motor, int8_t relative_speed);

void motor_stop(motor_t * motor);
void motor_disable(motor_t * motor);

void motor_set_pwm_ticks(motor_t * motor, uint16_t pwm_ticks);
void motor_set_mode(motor_t * motor);

motor_t * get_motor_top(void);
motor_t * get_motor_bottom(void);

void motors_send_speed(void);

#endif /* MOTORS_H_ */