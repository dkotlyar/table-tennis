/*
 * rollrot.c
 *
 * Created: 21.06.2016 11:26:38
 *  Author: Dmitriy
 */ 

#include "main.h"
#include "led.h"
#include "eeprom.h"

#include "rollrotation.h"

struct __roll_rotation__ {
	uint8_t state;
	uint8_t ready;
	float current_position;
	int16_t target_position;
	int16_t correction;
	
	uint8_t positioning;
	uint8_t direction;
	uint8_t enabled;
	uint8_t travel;
	uint8_t acceleration_enabled;
	uint8_t deceleration_enabled;
	
	uint16_t half_steps;
	uint16_t steps_count;
	
	float OCR_value;
	uint16_t OCR_start;
	uint16_t OCR_work;
	uint16_t OCR_finish;
	
	uint16_t steps_acceleration;
	uint16_t steps_deceleration;
	
	uint16_t start_freq;
	uint16_t work_freq;
	uint16_t finish_freq;
} rollrotation;

ISR (TIMER2_COMP_vect) {
	if (rollrotation.positioning) {
		return;
	}

	if (rollrotation.direction) {
		rollrotation.current_position += ROLLROTATION_DEGREE_BY_STEP / 2;
	} else {
		rollrotation.current_position -= ROLLROTATION_DEGREE_BY_STEP / 2;
	}
	
	if ((int16_t)rollrotation.current_position == rollrotation.target_position) {
		rollrotation_stop();
		rollrotation.ready = 1;
	}
	
	//rollrotation.half_steps++;
	//if (rollrotation.half_steps / 2 == rollrotation.steps_count - rollrotation.steps_deceleration) {
		//rollrotation_deceleration_enable();
	//}
}

ISR (INT6_vect) {
	rollrotation_is_min();
	rollrotation_is_max();
}

void rollrotation_init(void) {
	rollrotation.correction = eeprom_read_word(ROLLROTATION_CORRECTION_ADDR);
	
	ROLLROTATION_DDR		|= (1<<ROLLROTATION_PIN);
	ROLLROTATION_EN_DDR		|= (1<<ROLLROTATION_EN_PIN);
	ROLLROTATION_DIR_DDR	|= (1<<ROLLROTATION_DIR_PIN);
	ROLLROTATION_LED_DDR	|= ((1<<ROLLROTATION_LED_MIN_PIN) | (1<<ROLLROTATION_LED_MAX_PIN));
	ROLLROTATION_INT_DDR	&= ~((1<<ROLLROTATION_INT_MIN_PIN) | (1<<ROLLROTATION_INT_MAX_PIN));
	ROLLROTATION_READY_DDR	|= (1<<ROLLROTATION_READY_PIN);
	
	rollrotation.start_freq = ROLLROTATION_START_FREQ;
	rollrotation.work_freq = ROLLROTATION_WORK_FREQ;
	
	rollrotation.OCR_start = rollrotation_freq_to_ocr_ticks(ROLLROTATION_START_FREQ);
	rollrotation.OCR_work = rollrotation_freq_to_ocr_ticks(ROLLROTATION_WORK_FREQ);
	rollrotation.OCR_finish = rollrotation_freq_to_ocr_ticks(ROLLROTATION_FINISH_FREQ);
	
	EICRB	|= (1<<ISC61);
	//EICRB	|= (1<<ISC60);
	EIMSK	|= (1<<INT6);
	EIFR	|= (1<<INTF6);
	
	timer2_init();
	rollrotation_disable();

	rollrotation.target_position = 90;
}

void timer2_init(void) {
	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: 15,625 kHz
	// Mode: CTC top=OCR2A
	// OC2 output: Toggle on compare match
	// Timer Period: 3,328 ms
	// Output Pulse(s):
	// OC2 Period: 6,656 ms Width: 3,328 ms
	TCCR2	= (0<<WGM20) | (1<<WGM21) | 
			  (0<<COM21) | (1<<COM20) |
			  (0<<CS22) | (0<<CS21) | (0<<CS20);
	TCNT2	= 0;
	OCR2	= 0;
	
	TIMSK	|= (1<<OCIE2);
}

void rollrotation_save_zero(void) {
	rollrotation.correction -= rollrotation.current_position;
	eeprom_write_word(ROLLROTATION_CORRECTION_ADDR, rollrotation.correction);
}

void rollrotation_zero_reset(void) {
	rollrotation.current_position -= rollrotation.correction;
	rollrotation.correction = 0;
	eeprom_write_word(ROLLROTATION_CORRECTION_ADDR, 0);
}

void rollrotation_enable(void) {
	if (rollrotation.enabled) {
		return;
	}

	ROLLROTATION_EN_PORT |= (1<<ROLLROTATION_EN_PIN);
	rollrotation.enabled = 1;
	rollrotation.ready = 0;	
	rollrotation_positioning();
}

void rollrotation_disable(void) {
	ROLLROTATION_EN_PORT &= ~(1<<ROLLROTATION_EN_PIN);
	rollrotation_stop();
	rollrotation.enabled = 0;
	rollrotation.ready = 0;
}

void rollrotation_positioning(void) {	
	if (rollrotation_is_min()) {
		rollrotation.current_position = ROLLROTATION_ANGLE_MIN + rollrotation.correction;
		int16_t target = rollrotation.target_position;
		rollrotation.target_position = rollrotation.current_position;
		rollrotation_goto(target);
		return;
	}

	//rollrotation.current_position = 0x7FFF;
	//rollrotation.target_position = 0;

	rollrotation.positioning = 1;

	rollrotation_set_dir(0);
	rollrotation_start();
}

void rollrotation_goto(int16_t position) {
	//if (incline_state != STEP_MOTOR_READY) {
		//return;
	//}
	
	if (position > ROLLROTATION_ANGLE_MAX) {
		position = ROLLROTATION_ANGLE_MAX;
	} else if (position < ROLLROTATION_ANGLE_MIN) {
		position = ROLLROTATION_ANGLE_MIN;
	}

	if (rollrotation.target_position == position) {
		return;
	}

	if (rollrotation.positioning) {
		rollrotation.target_position = position;
		return;
	}
	
	if ((position > rollrotation.target_position) != rollrotation.direction) {
		rollrotation_stop();
		_delay_ms(50);
	}

	rollrotation.target_position = position;
	rollrotation_set_dir(position > rollrotation.current_position);
	rollrotation.steps_count = (uint16_t)round(abs(rollrotation.current_position - position) / ROLLROTATION_DEGREE_BY_STEP);
	
	rollrotation_acceleration_enable();
	rollrotation_start();
	
}

void rollrotation_set_dir(uint8_t direction) {
	rollrotation.direction = direction;
	if (direction) {
		ROLLROTATION_DIR_PORT |= (1<<ROLLROTATION_DIR_PIN);
	} else {
		ROLLROTATION_DIR_PORT &= ~(1<<ROLLROTATION_DIR_PIN);
	}
}

void rollrotation_start(void) {
	if ((rollrotation_is_min() & !rollrotation.direction) || (rollrotation_is_max() & rollrotation.direction)) {
		return;
	}
	
	rollrotation.OCR_value = rollrotation.OCR_start;
	
	rollrotation.half_steps = 0;
	#ifdef ROLLROTATION_ACCELERATION
		rollrotation.steps_acceleration = (uint16_t)((rollrotation.OCR_start - rollrotation.OCR_work) / ROLLROTATION_ACCELERATION);
		if (rollrotation.steps_acceleration > rollrotation.steps_count / 2) {
			rollrotation.steps_acceleration = rollrotation.steps_count / 2;
		}
	#endif
	#ifdef ROLLROTATION_DECELERATION
		rollrotation.steps_deceleration = (uint16_t)((rollrotation.OCR_finish - rollrotation.OCR_work) / ROLLROTATION_DECELERATION);
		if (rollrotation.steps_deceleration > rollrotation.steps_count / 2) {
			rollrotation.steps_deceleration = rollrotation.steps_count / 2;
		}
	#endif
	
	ROLLROTATION_OCR = (uint8_t)rollrotation.OCR_value;
	
	TCCR2 |= (1<<CS22) | (0<<CS21) | (1<<CS20);
	rollrotation.ready = 0;
	rollrotation.travel = 1;
}

void rollrotation_stop(void) {
	TCCR2 &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
	rollrotation.positioning = 0;
	rollrotation.travel = 0;
}

void rollrotation_change_speed(void) {
	#ifdef ROLLROTATION_ACCELERATION
		if (rollrotation.acceleration_enabled) {
			if (rollrotation.OCR_value > rollrotation.OCR_work) {
				rollrotation.OCR_value -= ROLLROTATION_ACCELERATION;
				ROLLROTATION_OCR = (uint16_t)rollrotation.OCR_value;
			}
		}
	#endif
	
	#ifdef ROLLROTATION_DECELERATION
		if (rollrotation.deceleration_enabled) {
			if (rollrotation.OCR_value < rollrotation.OCR_finish) {
				rollrotation.OCR_value += ROLLROTATION_DECELERATION;
				ROLLROTATION_OCR = (uint16_t)rollrotation.OCR_value;
			}
		}
	#endif
}

uint8_t rollrotation_freq_to_ocr_ticks(uint16_t freq) {
	return (uint8_t)((F_CPU / ROLLROTATION_PRESCALE) / 2 / freq - 1);
}

uint8_t rollrotation_is_max(void) {
	//uint8_t is_max = ((ROLLROTATION_INT_PORT & (1<<ROLLROTATION_INT_MAX_PIN)) == 0);
	//
	//if (is_max & rollrotation.direction) {
		//rollrotation_stop();
	//}
//
	//return is_max;

	return 0;
}

uint8_t rollrotation_is_min(void) {
	uint8_t is_min = ((ROLLROTATION_INT_PORT & (1<<ROLLROTATION_INT_MIN_PIN)) == 0);

	if (is_min & !rollrotation.direction) {
		rollrotation_stop();

		rollrotation.current_position = ROLLROTATION_ANGLE_MIN + rollrotation.correction;
		int16_t target_pos = rollrotation.target_position;
		rollrotation.target_position = rollrotation.current_position;
		rollrotation_goto(target_pos);
	}

	return is_min;
}


void rollrotation_acceleration_enable(void) {
	rollrotation_deceleration_disable();
	rollrotation.acceleration_enabled = 1;
}

void rollrotation_acceleration_disable(void) {
	rollrotation.acceleration_enabled = 0;
}

void rollrotation_deceleration_enable(void) {
	rollrotation_acceleration_disable();
	rollrotation.deceleration_enabled = 1;
}

void rollrotation_deceleration_disable(void) {
	rollrotation.deceleration_enabled = 0;
}


void rollrotation_led_min(void) {
	ROLLROTATION_LED_PORT |= (1<<ROLLROTATION_LED_MIN_PIN);
}

void rollrotation_led_max(void) {
	ROLLROTATION_LED_PORT |= (1<<ROLLROTATION_LED_MAX_PIN);
}

void rollrotation_led_disable(void) {
	ROLLROTATION_LED_PORT &= ~((1<<ROLLROTATION_LED_MIN_PIN) | (1<<ROLLROTATION_LED_MAX_PIN));
}

uint8_t rollrotation_check_ready(void) {
	if (!rollrotation.ready) {
		ROLLROTATION_READY_PORT |= (1<<ROLLROTATION_READY_PIN);
	} else {
		ROLLROTATION_READY_PORT &= ~(1<<ROLLROTATION_READY_PIN);
	}
	return rollrotation.ready;
}