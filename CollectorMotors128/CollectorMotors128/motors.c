/*
 * motors.c
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "motors.h"
#include "led.h"
#include "cmd.h"
#include "message.h"
#include "usart.h"

motors_t motors;

// Частотомер прерывание
ISR(CYMOMETR_TIMER_vect) {
	motor_period_tick(get_motor_top());
	motor_period_tick(get_motor_bottom());
}

ISR (HALL_TOP_vect) {
	motor_save_period(get_motor_top());
}

ISR (HALL_BOTTOM_vect) {
	motor_save_period(get_motor_bottom());
}

void motors_init(void) {
	timer1_init();
	timer3_init();
	cymometr_init();
	
	PWM_DDR		|= (1<<PWM_TOP_PIN) | (1<<PWM_BOT_PIN);
	MODE_DDR	|= (0b11<<MODE_TOP_PIN) | (0b11<<MODE_BOTTOM_PIN);
	SIG_DDR		|= (1<<SIG_TOP_PIN) | (1<<SIG_BOT_PIN);
		
	motors.top.OCR = &OCR_TOP;
	motors.top.MODE_PIN = MODE_TOP_PIN;
	
	motors.bottom.OCR = &OCR_BOTTOM;
	motors.bottom.MODE_PIN = MODE_BOTTOM_PIN;
	
	motor_disable(&motors.top);
	motor_disable(&motors.bottom);
}

// PWM
void timer1_init(void) {
	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 2000,000 kHz
	// Mode: Fast PWM top=ICR1
	// OC1A output: Non-Inverted PWM
	// OC1B output: Non-Inverted PWM
	// OC1C output: Disconnected
	// Prescale: 1
	TCCR1A	= (1<<COM1A1) | (0<<COM1A0) | (1<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (1<<WGM11) | (0<<WGM10);
	TCCR1B	= (0<<ICNC1) | (0<<ICES1) | (1<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
	TCNT1	= 0x00;
	ICR1	= PWM_TIMER_TOP;
	OCR1A	= 0x00;
	OCR1B	= 0x00;
	OCR1C	= 0x00;
}

// Частотомер
void timer3_init(void) {
	// Timer/Counter 3 initialization
	// Clock source: System Clock
	// Prescale: 1
	// Mode: CTC top=OCR3A
	TCCR3A	= (0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30);
	TCCR3B	= (0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (0<<CS31) | (1<<CS30);
	TCNT3	= 0;
	ICR3	= 0;
	OCR3A	= (F_CPU / CYMOMETR_FREQ) - 1;
	OCR3B	= 0;
	OCR3C	= 0;
	
	ETIMSK	|= (1<<OCIE3A);
}

void cymometr_init(void) {
	// External Interrupt(s) initialization
	// INT0: On
	// INT0 Mode: Falling Edge
	// INT1: On
	// INT1 Mode: Falling Edge
	EICRA	|=	(1<<ISC11) | (1<<ISC01);
	#ifdef RISING_EDGE_INT
	EICRA	|=	(1<<ISC10) | (1<<ISC00);
	#endif
	EIMSK	|= (1<<INT1) | (1<<INT0);
	EIFR	|= (1<<INTF1) | (1<<INTF0);
}


void motor_period_tick(motor_t * motor) {
	if (motor->temp_period_ticks == MAX_CYMOMETR_COUNT) {
		motor->actual_speed = 0;
		motor_save_period(motor);
	} else {
		motor->temp_period_ticks++;
	}
}

void motor_save_period(motor_t * motor) {
	motor->cymometr_status = motor->cymometr_status << 1 | 1;
	motor->period_ticks = motor->temp_period_ticks;
	motor->temp_period_ticks = 0;
}

void regulate_all_motors(void) {
	motor_regulator(get_motor_top());
	motor_regulator(get_motor_bottom());
}

void motor_regulator(motor_t * motor) {
	if ((motor->cymometr_status & 1) == 0) {
		return;
	}
	motor->cymometr_status = motor->cymometr_status << 1;
	
	if (motor->period_ticks == MAX_CYMOMETR_COUNT) {
		motor->actual_speed = 0;
	} else {
		motor->actual_speed = (uint16_t)((uint32_t)CYMOMETR_FREQ * 60 / motor->period_ticks)*0.98;
	}
	
	if (motor->absolute_speed == 0) {
		return;
	}
	
	motor_speed_analyze(motor);
	
	motor_set_pwm_ticks(motor, speed_regulator(motor));
}

void motor_speed_analyze(motor_t * motor) {
	if (abs(motor->absolute_speed - motor->actual_speed) < SPEED_EPSILON) {
		motor->ready = 1;
	} else {
		motor->ready = 0;
	}
}

uint16_t speed_regulator(motor_t * motor) {
	float pwm_ticks = (float)motor->pwm_ticks;
	int16_t eps = motor->absolute_speed - motor->actual_speed;
	
	// u(t) = P(t) + I(t) + D(t);
	// P(t) = Kp * e(t);
	// I(t) = I (t - 1) + Ki * e(t);
	// D(t) = Kd * [e(t) - e(t - 1)];
	
	float p = 0, i = 0, d = 0;
	#ifdef PROPORTIONAL_ADJUSTMENT_FACTOR
		p = ((float)PROPORTIONAL_ADJUSTMENT_FACTOR * eps);
	#endif
	#ifdef INTEGRATED_ADJUSTMENT_FACTOR
		i = ((float)INTEGRATED_ADJUSTMENT_FACTOR * eps + motor->last_integr);
	#endif
	#ifdef DIFFERENTIAL_ADJUSTMENT_FACTOR
		d = ((float)DIFFERENTIAL_ADJUSTMENT_FACTOR * (eps - motor->last_eps));
	#endif
	
	motor->last_eps = eps;
	motor->last_integr = i;
	
	pwm_ticks += p + i + d;
	
	if (pwm_ticks > PWM_TIMER_TOP) {
		pwm_ticks = PWM_TIMER_TOP;
	} else if (pwm_ticks < 0) {
		pwm_ticks = 0;
	}
	
	return (uint16_t)pwm_ticks;
}

void motors_check_ready(void) {
	if (!motors.top.ready) {
		SIG_PORT |= (1<<SIG_TOP_PIN);
	} else {
		SIG_PORT &= ~(1<<SIG_TOP_PIN);
	}
	
	if (!motors.bottom.ready) {
		SIG_PORT |= (1<<SIG_BOT_PIN);
	} else {
		SIG_PORT &= ~(1<<SIG_BOT_PIN);
	}
		
	#ifdef SHOW_READY_ON_LED
	if (motors.top.ready && motors.bottom.ready) {
		led_enable();
	} else {
		led_disable();
	}
	#endif
}


uint16_t speed_to_pwm(uint16_t speed) {
						// 0 .. MOTOR_MAX_SPEED
	uint16_t ocr = (uint16_t)((uint32_t)speed * PWM_TIMER_TOP / MOTOR_MAX_SPEED);

	if (ocr > PWM_TIMER_TOP) {
		ocr = PWM_TIMER_TOP;
	}
	return ocr;
}

uint16_t width_to_pwm(uint8_t width) {
				       // 0 .. 100
	uint16_t ocr = (((uint16_t)width * PWM_TIMER_TOP) / 100);
	
	if (ocr > PWM_TIMER_TOP) {
		ocr = PWM_TIMER_TOP;
	}
	
	return ocr;
}


void motor_set_relative_speed(motor_t * motor, int8_t relative_speed) {
	motor->absolute_speed = 0;
	motor->ready = 1;
	
	if (relative_speed == 0) {
		motor_stop(motor);
		motor_set_mode(motor);
		return;
	} else if (relative_speed > 0) {
		motor->status = ROTATE_FORWARD;
		motor->relative_speed = relative_speed;
	} else {
		motor->status = ROTATE_BACKWARD;
		motor->relative_speed = -relative_speed;
	}
	motor_set_pwm_ticks(motor, width_to_pwm(motor->relative_speed));
	motor_set_mode(motor);
}

void motor_set_absolute_speed(motor_t * motor, int16_t absolute_speed) {
	motor->relative_speed = 0;
	motor->ready = 0;
	
	if (absolute_speed == 0) {
		motor_stop(motor);
		motor_set_mode(motor);
		return;
	} else if (absolute_speed > 0) {
		motor->status = ROTATE_FORWARD;
		motor->absolute_speed = absolute_speed;
	} else {
		motor->status = ROTATE_BACKWARD;
		motor->absolute_speed = -absolute_speed;
	}
	motor_set_pwm_ticks(motor, speed_to_pwm(motor->absolute_speed));
	motor_set_mode(motor);
}

void motor_stop(motor_t * motor) {
	motor->status = MOTOR_STOP;
	motor_set_mode(motor);
}

void motor_disable(motor_t * motor) {
	motor->status = MOTOR_DISABLE;
	motor_set_mode(motor);
}


void motor_set_pwm_ticks(motor_t * motor, uint16_t pwm_ticks) {
	motor->pwm_ticks = pwm_ticks;
	*motor->OCR = pwm_ticks;
}

void motor_set_mode(motor_t * motor) {
	MODE_PORT &= ~(0b11<<motor->MODE_PIN);
	MODE_PORT |= (motor->status << motor->MODE_PIN);
	
	if (motor->status == MOTOR_DISABLE || motor->status == MOTOR_STOP) {
		motor->pwm_ticks = 0;
		motor->absolute_speed = 0;
		motor->relative_speed = 0;

		if (motor->status == MOTOR_DISABLE) {
			motor->ready = 0;
		} else {
			motor->ready = 1;
		}
	}
}


motor_t * get_motor_top(void) {
	return &motors.top;
}

motor_t * get_motor_bottom(void) {
	return &motors.bottom;
}


void motors_send_speed(void) {	
	static uint16_t last_top_speed = 0;
	static uint16_t last_bottom_speed = 0;
	
	if (last_bottom_speed != motors.bottom.absolute_speed) {
		last_bottom_speed = motors.bottom.actual_speed;
		
		uint8_t length = (sizeof(uint16_t) + 1);
		uint8_t data[length];
		*data = BR_SPEED;
		memcpy(data+1, &last_bottom_speed, sizeof(uint16_t));
		usart_remove_transmit(*data);
		add_transmit(data, length);
	}
	if (last_top_speed != motors.top.absolute_speed) {
		last_top_speed = motors.top.actual_speed;
		
		uint8_t length = (sizeof(uint16_t) + 1);
		uint8_t data[length];
		*data = TR_SPEED;
		memcpy(data+1, &last_top_speed, sizeof(uint16_t));
		usart_remove_transmit(*data);
		add_transmit(data, length);
	}
}