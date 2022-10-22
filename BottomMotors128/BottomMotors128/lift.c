/*
 * lift.c
 *
 * Created: 24.06.2016 20:56:32
 *  Author: Dmitriy
 */ 

#include "main.h"
#include "lift.h"

#include "step_motor.h"

#include "led.h"

step_motor_t lift;

ISR (TIMER3_COMPA_vect) {
	if (lift.positioning)
		return;

	if (lift.direction) {
		lift.current_position += LIFT_DISTANCE_PER_STEP / 2;
	} else {
		lift.current_position -= LIFT_DISTANCE_PER_STEP / 2;
	}
	
	if ((int16_t)lift.current_position == lift.target_position) {
		lift_stop();
		lift.ready = 1;
	}

	lift.half_steps++;
	if (lift.half_steps / 2 == lift.steps_count - lift.steps_deceleration) {
		lift_deceleration_enable();
	}
}

ISR (INT7_vect) {
	if (lift_is_min() && lift.direction == 0) {
		lift_stop();
	}

	lift_is_max();

	//if (lift_is_min() & !lift.direction) {
		//lift_stop();
//
		//lift.current_position = LIFT_MINIMUM_POSITION;
		//int16_t target = lift.target_position;
		//lift.target_position = lift.current_position;
		//lift_goto(target);
	//}
	//if (lift_is_max() & lift.direction) {
		//lift_stop();
	//}
}

void lift_init(void) {
	lift.current_position = 0;
	lift.start_freq = LIFT_START_FREQ;
	lift.work_freq = LIFT_WORK_FREQ;
	lift.finish_freq = LIFT_FINISH_FREQ;
	
	lift.OCR_start = lift_freq_to_ocr_tick(LIFT_START_FREQ);
	lift.OCR_work = lift_freq_to_ocr_tick(LIFT_WORK_FREQ);
	lift.OCR_finish = lift_freq_to_ocr_tick(LIFT_FINISH_FREQ);
	
	LIFT_ENABLE_DDR |= (1<<LIFT_ENABLE_PIN);
	LIFT_DIR_DDR	|= (1<<LIFT_DIR_PIN);
	LIFT_LED_DDR	|= (1<<LIFT_LED_MIN_PIN) | (1<<LIFT_LED_MAX_PIN);
	LIFT_INT_DDR	&= ~((1<<LIFT_INT_MIN_PIN) | (1<<LIFT_INT_MAX_PIN));
	LIFT_READY_DDR	|= (1<<LIFT_READY_PIN);
	
	EICRB	|= (1<<ISC71);
	//EICRB	|= (1<<ISC70);
	EIMSK	|= (1<<INT7);
	EIFR	|= (1<<INTF7);
	
	timer3_init();
	lift_disable();

	lift.target_position = 762;
}

void timer3_init(void) {
	LIFT_DDR |= (1<<LIFT_PIN);
	
	// Timer/Counter 3 initialization
	// Clock source: System Clock
	// Clock value: 2000,000 kHz
	// Mode: CTC top=OCR3A
	// OC3A output: Toggle on compare match
	// OC3B output: Disconnected
	// OC3C output: Disconnected
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer Period: 0,5 us
	// Output Pulse(s):
	// OC3A Period: 1 us Width: 0,5 us
	// Timer3 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: On
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off
	TCCR3A	= (0<<COM3A1) | (1<<COM3A0) | 
			  (0<<COM3B1) | (0<<COM3B0) | 
			  (0<<COM3C1) | (0<<COM3C0) | 
			  (0<<WGM31) | (0<<WGM30);
	TCCR3B	= (0<<ICNC3) | (0<<ICES3) | 
			  (0<<WGM33) | (1<<WGM32) | 
			  (0<<CS32) | (0<<CS31) | (0<<CS30);
	TCNT3	= 0x00;
	ICR3	= 0x00;
	OCR3A	= 0x00;
	OCR3B	= 0x00;
	OCR3C	= 0x00;
	
	ETIMSK	|= (1<<OCIE3A);

}

uint16_t lift_freq_to_ocr_tick(uint16_t freq) {
	return (uint16_t)(F_CPU / LIFT_PRESCALE / 2 / freq) - 1;
}

void lift_goto(int16_t position) {	
	if (position < LIFT_MINIMUM_POSITION) {
		position = LIFT_MINIMUM_POSITION;
	} else if (position > LIFT_MAXIMUM_POSITION) {
		position = LIFT_MAXIMUM_POSITION;
	}
	
	if (lift.target_position == position) {
		return;
	}

	if (lift.positioning) {
		lift.target_position = position;
		return;
	}
	
	if ((position > lift.target_position) != lift.direction) {
		lift_stop();
		_delay_ms(50);
	}
	lift_set_dir(position > lift.current_position);
	lift.steps_count = (uint16_t)round(abs(lift.current_position - position) / LIFT_DISTANCE_PER_STEP);
	
	lift_acceleration_enable();
	lift_start();
	
	lift.target_position = position;
}

void lift_positioning(void) {
	lift_set_dir(0);

	if (lift_is_min()) {
		//lift.current_position = LIFT_MINIMUM_POSITION;
		//int16_t target = lift.target_position;
		//lift.target_position = lift.current_position;
		//lift_goto(target);
		return;
	}

	//lift.current_position = 0x7FFF;
	//lift.target_position	= 0;

	lift.positioning = 1;
	lift_start();
}

void lift_enable(void) {
	if (lift.enabled) {
		return;
	}

	LIFT_ENABLE_PORT &= ~(1<<LIFT_ENABLE_PIN);
	lift.enabled = 1;
	lift.ready = 0;
	lift_positioning();
}

void lift_disable(void) {
	LIFT_ENABLE_PORT |= (1<<LIFT_ENABLE_PIN);
	lift_stop();
	lift.enabled = 0;
	lift.ready = 0;
}

void lift_stop(void) {
	TCCR3B &= ~((1<<CS32) | (1<<CS31) | (1<<CS30));
	lift.positioning = 0;
	lift.travel = 0;
}

void lift_start(void) {
	if ((lift_is_min() & !lift.direction) || (lift_is_max() & lift.direction)) {
		return;
	}
	
	lift.OCR_value = lift.OCR_start;
	
	lift.half_steps	= 0;
	lift.steps_acceleration = (uint16_t)(abs(lift.OCR_start - lift.OCR_work) / LIFT_ACCELERATION);
	lift.steps_deceleration = (uint16_t)(abs(lift.OCR_work - lift.OCR_finish) / LIFT_DECELERATION);
	
	if (lift.steps_acceleration > lift.steps_count / 2) {
		lift.steps_acceleration = lift.steps_count / 2;
	}
	if (lift.steps_deceleration > lift.steps_count / 2) {
		lift.steps_deceleration = lift.steps_count / 2;
	}

	LIFT_OCR = (uint16_t)lift.OCR_value;
	
	TCCR3B |= ((0<<CS32) | (1<<CS31) | (0<<CS30));
	lift.ready = 0;
	lift.travel = 1;
}

uint8_t lift_is_max(void) {
	uint8_t is_max = ((LIFT_INT_PORT & (1<<LIFT_INT_MAX_PIN)) == 0);

	if (is_max & lift.direction) {
		lift_stop();
	}

	return is_max;
}

uint8_t lift_is_min(void) {
	uint8_t is_min = ((LIFT_INT_PORT & (1<<LIFT_INT_MIN_PIN)) == 0);
	
	if (is_min && lift.direction == 0) {
		lift_stop();

		lift.current_position = LIFT_MINIMUM_POSITION;
		int16_t target = lift.target_position;
		lift.target_position = lift.current_position;
		lift_goto(target);
	}

	return is_min;
}

void lift_set_dir(uint8_t direction) {
	lift.direction = direction;
	if (direction) {
		LIFT_DIR_PORT &= ~(1<<LIFT_DIR_PIN);
	} else {
		LIFT_DIR_PORT |= (1<<LIFT_DIR_PIN);
	}
}

void lift_change_speed(void) {
	if (lift.acceleration_enabled) {
		if (lift.OCR_value > lift.OCR_work) {
			lift.OCR_value -= LIFT_ACCELERATION;
			LIFT_OCR = (uint16_t)lift.OCR_value;
		}
	}
	
	if (lift.deceleration_enabled) {
		if (lift.OCR_value < lift.OCR_finish) {
			lift.OCR_value += LIFT_DECELERATION;
			LIFT_OCR = (uint16_t)lift.OCR_value;
		}
	}
}

void lift_acceleration_enable(void) {
	lift_deceleration_disable();
	lift.acceleration_enabled = 1;
}

void lift_acceleration_disable(void) {
	lift.acceleration_enabled = 0;
}

void lift_deceleration_enable(void) {
	lift_acceleration_disable();
	lift.deceleration_enabled = 1;
}

void lift_deceleration_disable(void) {
	lift.deceleration_enabled = 0;
}

void lift_led_min(void) {
	LIFT_LED_PORT |= (1<<LIFT_LED_MIN_PIN);
}

void lift_led_max(void) {
	LIFT_LED_PORT |= (1<<LIFT_LED_MAX_PIN);
}

void lift_led_disable(void) {
	LIFT_LED_PORT &= ~((1<<LIFT_LED_MIN_PIN) | (1<<LIFT_LED_MAX_PIN));
}

uint8_t lift_check_ready(void) {
	if (!lift.ready) {
		LIFT_READY_PORT |= (1<<LIFT_READY_PIN);
	} else {
		LIFT_READY_PORT &= ~(1<<LIFT_READY_PIN);
	}
	return lift.ready;
}

uint8_t lift_is_travel(void) {
	return lift.travel;
}