/*
 * step_motors.c
 *
 * Created: 10.09.2015 9:47:40
 *  Author: Dmitriy
 */

#include "main.h"
#include "motors.h"



float rotation_error = 16.0;
float rotation_current = 0.0;
float rotation_max = 0.0;
float rotation_min = 0.0;
uint8_t rotation_dir_clockwise		= 0;
uint8_t rotation_by_step			= 0;
uint8_t rotation_enabled			= 0;
uint8_t rotation_positioning		= 0;
uint8_t rotation_state				= STEP_MOTOR_UNKNOW;
uint16_t rotation_step_count		= 0;
uint16_t rotation_step_count_max	= 0;
uint16_t rotation_ctc_value			= 0;
uint16_t rotation_ctc_value_start	= 0;
uint16_t rotation_ctc_value_end		= 0;

//TIMER1_COMPA_vect
ISR (TIMER2_COMP_vect) {
	static uint8_t counter = 0;
	counter++;
	if (counter % 2 > 0) return;
	
	rotation_step_count++;
	if (rotation_by_step && rotation_step_count >= rotation_step_count_max) {
		//if (rotation_step_count < rotation_step_count_max / 2 && ) {
			//rotation_ctc_value--;
			//OCR2 = rotation_ctc_value;
		//} else if (rotation_step_count > rotation_step_count_max / 2) {
			//rotation_ctc_value++;
			//OCR2 = rotation_ctc_value;
		//}
		stop_rotation();
	}
}

// INT6
ISR (INT2_vect) {
	if (rotation_must_stop()) {
		stop_rotation();
	}
}

ISR (INT3_vect) {
	if (rotation_must_stop()) {
		stop_rotation();
	}
}


void init_step_motors(void) {
	DDRA	|= (1<<DDA5) | (1<<DDA4) | (1<<DDA1) | (1<<DDA0);
	
	init_interrupt_stop();
	init_lift_timer();
	init_rotation_timer();
	
	//set_lift_start_freq(LIFT_START_FREQ);
	//set_lift_end_freq(LIFT_END_FREQ);
	
	set_rotation_start_freq(ROTATION_START_FREQ);
	set_rotation_end_freq(ROTATION_END_FREQ);

	//disable_lift();
	disable_rotation();
}

void init_rotation_timer(void) {
	DDRB	|= (1<<DDB7);
	
	TCCR2	|=	(0<<WGM20) | (1<<WGM21) |
				(0<<COM21) | (1<<COM20) | 
				(0<<CS22) | (0<<CS21) | (0<<CS20);
	TCNT2	= 0x00;
	OCR2	= 0x00;
	
	TIMSK	|= (1<<OCIE2);
}

void init_interrupt_stop(void) {
	DDRD	&= ~((1<<DDD2) | (1<<DDD3));
	DDRE	&= ~((1<<DDE6) | (1<<DDE7));
	
	EICRA	|= (1<<ISC31) | (1<<ISC21);
	EICRB	|= (1<<ISC71) | (1<<ISC61);
	EIMSK	|= (1<<INT7) | (1<<INT6) | (1<<INT3) | (1<<INT2);
	EIFR	|= (1<<INTF7) | (1<<INTF6) | (1<<INTF3) | (1<<INTF2);
}




uint16_t rotation_freq_to_ctc(uint16_t freq) {
	return (int)((F_CPU / 256) / freq); // 256 предделитель
}

void set_rotation_start_freq(uint16_t freq) {
	rotation_ctc_value_start = rotation_freq_to_ctc(freq);
	//if (lift_ctc_value_end < lift_ctc_value_start)
	//lift_ctc_value_end = lift_ctc_value_start;
}

void set_rotation_end_freq(uint16_t freq) {
	rotation_ctc_value_end = rotation_freq_to_ctc(freq);
	//if (lift_ctc_value_end < lift_ctc_value_start)
	//lift_ctc_value_start = lift_ctc_value_end;
}

void set_rotation(float _rotation) {
	if (rotation_state != STEP_MOTOR_READY)
		return;
		
	if (rotation_current == _rotation) {
		//registrate_function(&rotation_ready);
		return;
	}
	
	
	if (_rotation < rotation_min)
		_rotation = rotation_min;
	if (_rotation > rotation_max)
		_rotation = rotation_max;
	
	set_rotation_dir(_rotation > rotation_current);
	uint16_t step_count = 0;
	if (rotation_current < _rotation) {
		step_count = (uint16_t)round((_rotation - rotation_current) / get_rotation_degree_by_step());
	} else if (rotation_current > _rotation) {
		step_count = (uint16_t)round((rotation_current - _rotation) / get_rotation_degree_by_step());
	}
	
	set_rotation_step_count(step_count);
	start_rotation(1);
	
	rotation_current = _rotation;
}

void positioning_rotation_async(void) {
}

void enable_rotation(void) {
	PORTA &= ~(1<<PINA5);
}

void disable_rotation(void) {
	PORTA |= (1<<PINA5);
	rotation_state = STEP_MOTOR_UNKNOW;
}

void stop_rotation(void) {
	TCCR2 &= ~((1<<CS32) | (1<<CS31) | (1<<CS30));
	rotation_by_step = 0;
	rotation_enabled = 0;
	
	//if (rotation_state == STEP_MOTOR_READY)
		//registrate_function(&rotation_ready);
}

void start_rotation(uint8_t by_step) {
	if (rotation_must_stop()) {
		return;
	}

	TCCR2 |= (1<<CS22) | (0<<CS21) | (0<<CS20);
	rotation_step_count	= 0;
	rotation_by_step	= by_step;
	rotation_enabled	= 1;
	rotation_ctc_value = rotation_ctc_value_start;
	OCR2 = rotation_ctc_value;
	
	//lift_ctc_value = lift_freq_to_ctc(LIFT_GARANTED_FREQ);
	//OCR3A = lift_ctc_value;
	//lift_acceleration_en = 0;
}

uint8_t rotation_is_max(void) {
	return ((PIND & ROTATION_STOP_CLOCKWISE) == 0) & rotation_dir_clockwise;
}

uint8_t rotation_is_min(void) {
	return ((PIND & ROTATION_STOP_ANTICLOCKWISE) == 0) & !rotation_dir_clockwise;
}

uint8_t rotation_must_stop(void) {
	return rotation_is_max() || rotation_is_min();
}

void set_rotation_dir(uint8_t is_clockwise) {
	rotation_dir_clockwise = is_clockwise;
	if (is_clockwise) {
		PORTA |= (1<<PINA1);
	} else {
		PORTA &= ~(1<<PINA1);
	}
}

void set_rotation_step_count(uint16_t step) {
	rotation_step_count_max = step;
}

uint16_t get_rotation_step_count(void) {
	return rotation_step_count;
}

uint8_t rotation_is_enabled(void) {
	return rotation_enabled;
}

uint8_t rotation_is_clockwise(void) {
	return rotation_dir_clockwise;
}

float get_rotation_degree_by_step(void) {
	// 15/45 = 1/3 = 0,333
	// 44/124 = 0,3548387
	float gear_ratio		= 0.333;
	float degree_by_step	= 0.9 * gear_ratio;
	
	return degree_by_step;
}

void rotation_ready(void) {
}