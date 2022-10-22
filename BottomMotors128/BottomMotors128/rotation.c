/*
 * rotation.c
 *
 * Created: 25.06.2016 20:56:32
 *  Author: Dmitriy
 */ 

#include "main.h"
#include "rotation.h"

#include "step_motor.h"
#include "led.h"
#include "millis.h"
#include "usart.h"
#include "lift.h"
#include <math.h>

step_motor_t rotation;

int16_t zero_angle;
uint16_t max_intensity;
uint16_t min_distance;
uint16_t last_distance;

ISR (TIMER1_COMPA_vect) {
	if (rotation.direction) {
		rotation.current_position += ROTATION_DEGREE_BY_STEP / 2;
	} else {
		rotation.current_position -= ROTATION_DEGREE_BY_STEP / 2;
	}
	
	if ((int16_t)rotation.current_position == rotation.target_position) {
		rotation_stop();
		rotation.ready = 1;
	}
	
	rotation.half_steps++;
	if (rotation.half_steps / 2 == rotation.steps_count - rotation.steps_deceleration) {
		rotation_deceleration_enable();
	}
}

ISR (INT6_vect) {
	if (rotation_is_min() & !rotation.direction) {
		rotation_stop();
	}
	if (rotation_is_max() & rotation.direction) {
		rotation_stop();
	}
}

void rotation_init(void) {
	rotation.current_position = 0;
	rotation.start_freq = ROTATION_START_FREQ;
	rotation.work_freq = ROTATION_WORK_FREQ;
	
	rotation.OCR_start = rotation_freq_to_ocr_tick(ROTATION_START_FREQ);
	rotation.OCR_work = rotation_freq_to_ocr_tick(ROTATION_WORK_FREQ);
	rotation.OCR_finish = rotation_freq_to_ocr_tick(ROTATION_FINISH_FREQ);
	
	ROTATION_ENABLE_DDR |= (1<<ROTATION_ENABLE_PIN);
	ROTATION_DIR_DDR	|= (1<<ROTATION_DIR_PIN);
	ROTATION_LED_DDR	|= (1<<ROTATION_LED_MIN_PIN) | (1<<ROTATION_LED_MAX_PIN);
	ROTATION_INT_DDR	&= ~((1<<ROTATION_INT_MIN_PIN) | (1<<ROTATION_INT_MAX_PIN));
	ROTATION_READY_DDR	|= (1<<ROTATION_READY_PIN);
	
	EICRB	|= (1<<ISC61); // Fall
	//EICRB	|= (1<<ISC60); // Any Change
	EIMSK	|= (1<<INT6);
	EIFR	|= (1<<INTF6);
	
	BEACON_DDR &=  ~(1<<BEACON_IR);
	BEACON_DDR |=   (1<<BEACON_TRIG);
	BEACON_DDR &=  ~(1<<BEACON_ECHO);

	timer1_init();

	rotation_disable();
}

void timer1_init(void) {
	ROTATION_DDR |= (1<<ROTATION_PIN);
	
	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 2000,000 kHz
	// Mode: CTC top=OCR1A
	// OC1A output: Toggle on compare match
	// OC1B output: Disconnected
	// OC1C output: Disconnected
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer Period: 0,5 us
	// Output Pulse(s):
	// OC1A Period: 1 us Width: 0,5 us
	// Timer1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: On
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off
	TCCR1A	= (0<<COM1A1) | (1<<COM1A0) | 
			  (0<<COM1B1) | (0<<COM1B0) | 
			  (0<<COM1C1) | (0<<COM1C0) | 
			  (0<<WGM11) | (0<<WGM10);
	TCCR1B	= (0<<ICNC1) | (0<<ICES1) | 
			  (0<<WGM13) | (1<<WGM12) | 
			  (0<<CS12) | (0<<CS11) | (0<<CS10);
	TCNT1	= 0x00;
	ICR1	= 0x00;
	OCR1A	= 0x00;
	OCR1B	= 0x00;
	OCR1C	= 0x00;
	
	TIMSK	|= (1<<OCIE1A);

}


uint16_t rotation_freq_to_ocr_tick(uint16_t freq) {
	return (uint16_t)(F_CPU / ROTATION_PRESCALE / 2 / freq) - 1;
}

void rotation_goto(int16_t position) {	
	if (position < ROTATION_MINIMUM_POSITION) {
		position = ROTATION_MINIMUM_POSITION;
	} else if (position > ROTATION_MAXIMUM_POSITION) {
		position = ROTATION_MAXIMUM_POSITION;
	}
	
	if (rotation.target_position == position) {
		return;
	}
	
	if ((position > rotation.target_position) != rotation.direction) {
		rotation_stop();
		_delay_ms(50);
	}
	rotation_set_dir(position > rotation.current_position);
	rotation.steps_count = (uint16_t)round(abs(rotation.current_position - position) / ROTATION_DEGREE_BY_STEP);
	
	rotation_acceleration_enable();
	rotation_start();
	
	rotation.target_position = position;
}

void rotation_positioning(void) {
	rotation.current_position = 0;
	rotation.target_position = 0;
	rotation.ready = 1;

}

void rotation_enable_positioning(void) {
	rotation.positioning = 1;
}

void rotation_positioning_second(void) {

	static uint8_t step = 0;
	char str[30];

	static int16_t left_beacon_start;
	static int16_t left_beacon_end;
	static int16_t right_beacon_start;
	static int16_t right_beacon_end;

	static int16_t right_angle, left_angle;

	static uint32_t right_time, left_time;
	
	static float right, left;
	static float alpha;

	uint64_t time_start, time_end, time_sum;

	if (!rotation.positioning) {
		return;
	}

	if (lift_is_travel() || rotation.travel) {
		return;
	}

	switch (step) {
		case 0:
			lift_enable();
			lift_goto(762);

			rotation.current_position = -180;
			rotation.target_position = 180;
			rotation_set_dir(1);
			//rotation.steps_count = (uint16_t)round(360 / ROTATION_DEGREE_BY_STEP);
			//rotation_acceleration_enable();
			rotation_start();
			
			step++;
			return;

		case 1:
			rotation_set_dir(0);
			rotation.current_position = rotation.target_position;
			step++;
			return;

		case 2:
			if ((BEACON_PIN & (1<<BEACON_IR)) == 0) {
				rotation.target_position--;
				rotation_start();
				_delay_us(STEP_DELAY_US);
			} else {
				step++;
			}
			return;

		case 3:
			right_beacon_start = rotation.target_position;
			step++;
			return;

		case 4:
			if ((BEACON_PIN & (1<<BEACON_IR)) != 0) {
				rotation.target_position--;
				rotation_start();
				_delay_us(STEP_DELAY_US);
			} else {
				step++;
			}
			return;

		case 5:
			right_beacon_end = rotation.target_position;
			right_angle = (right_beacon_start + right_beacon_end) / 2;
			step++;
			return;

		case 6:
			//rotation_set_dir(1);
			//rotation.target_position = right_angle;
			//rotation_start();

			rotation_goto(right_angle);

			step++;
			return;

		case 7:
			_delay_ms(500);
			time_sum = 0;
			for (uint8_t i = 0; i < ULTRASONIC_COUNT; i++) {

				time_start = micros();
				BEACON_PORT |= (1<<BEACON_TRIG);
				
				while ((BEACON_PIN & (1<<BEACON_ECHO)) == 0) ;
				time_end = micros();
				
				BEACON_PORT &= ~(1<<BEACON_TRIG);
				_delay_ms(100);

				time_sum += (time_end - time_start);
			}

			//uint32_t time_buffer;
			//for (uint8_t i = 0; i < 5; i++) {
				//for (uint8_t j = 0; j < 5; j++) {
					//if (time_array[i] < time_array[j]) {
						//time_buffer = time_array[i];
						//time_array[i] = time_array[j];
						//time_array[j] = time_buffer;
					//}
				//}
			//}
			
			//right_time = time_end - time_start;
			//right_time = time_array[2];
			right_time = time_sum / ULTRASONIC_COUNT;

			right = (float)right_time * SOUND_CONST * sqrt(KELVIN_ZERO + TEMPERATURE) / 1000;
			sprintf(str, "right, mm: %u", (uint16_t)right);
			usart_print(get_usart1(), str);

			step++;
			return;

		case 8:
			rotation_set_dir(0);
			step++;
			return;

		case 9:
			if ((BEACON_PIN & (1<<BEACON_IR)) == 0) {
				rotation.target_position--;
				rotation_start();
				_delay_us(STEP_DELAY_US);
			} else {
				step++;
			}
			return;

		case 10:
			left_beacon_start = rotation.target_position;
			step++;
			return;

		case 11:
			if ((BEACON_PIN & (1<<BEACON_IR)) != 0) {
				rotation.target_position--;
				rotation_start();
				_delay_us(STEP_DELAY_US);
			} else {
				step++;
			}
			return;

		case 12:
			left_beacon_end = rotation.target_position;
			left_angle = (left_beacon_start + left_beacon_end) / 2;
			step++;
			return;

		case 13:
			//rotation_set_dir(1);
			//rotation.target_position = left_angle;
			//rotation_start();

			rotation_goto(left_angle);

			step++;
			return;

		case 14:
			_delay_ms(500);
			time_sum = 0;
			for (uint8_t i = 0; i < ULTRASONIC_COUNT; i++) {

				time_start = micros();
				BEACON_PORT |= (1<<BEACON_TRIG);
				
				while ((BEACON_PIN & (1<<BEACON_ECHO)) == 0) ;
				time_end = micros();
				
				BEACON_PORT &= ~(1<<BEACON_TRIG);
				_delay_ms(100);

				time_sum += (time_end - time_start);
			}
			
			//left_time = time_end - time_start;
			left_time = time_sum / ULTRASONIC_COUNT;
			left = (float)left_time * SOUND_CONST * sqrt(KELVIN_ZERO + TEMPERATURE) / 1000;
			
			sprintf(str, "left, mm: %u", (uint16_t)left);
			usart_print(get_usart1(), str);

			step++;
			return;

		case 15:
			//sprintf(str, "r_angl: %d, l_angl: %d", right_angle, left_angle);
			//usart_print(get_usart1(), str);

			;

			//float r = (float)right;
			//float l = (float)left;
			float a = (float)1525;

			alpha = acos((right*right + a*a - left*left) / (2*right*a)) * 180 / M_PI;
			//float tetha = right_angle - left_angle;
			//float beta = 180 - (tetha + alpha);
			
			//sprintf(str, "%d / %d", (uint16_t)(r*r + a*a - l*l), (uint16_t)(2*r*a));
			//usart_print(get_usart1(), str);

			// a = 55, b = 56, O = 69
			sprintf(str, "alpha: %d", (int16_t)alpha);
			usart_print(get_usart1(), str);

			//rotation_set_dir(right + alpha > rotation.target_position);
			//rotation.target_position = right + (90 - alpha);
			//rotation_start();
			rotation_goto(right_angle - (90 - alpha));

			step++;
			return;

		case 16:
			//rotation.current_position = 0;
			//rotation.target_position = 0;
			;
			
			float y = 1525/2 - (float)right * cos(alpha / 180 * M_PI);
			float x = (float)right * sin(alpha / 180 * M_PI);

			sprintf(str, "x: %d, y: %d", (int16_t)x, (int16_t)y);
			usart_print(get_usart1(), str);
			
			step++;
			return;

		default:
			rotation.positioning = 0;
			step = 0;
			return;
	}

}


void rotation_enable(void) {
	ROTATION_ENABLE_PORT &= ~(1<<ROTATION_ENABLE_PIN);
	rotation.enabled = 1;
	rotation.ready = 0;
	rotation_positioning();
}

void rotation_disable(void) {
	ROTATION_ENABLE_PORT |= (1<<ROTATION_ENABLE_PIN);
	rotation_stop();
	rotation.enabled = 0;
	rotation.ready = 0;
	//lift.state = STEP_MOTOR_UNKNOW;
}

void rotation_stop(void) {
	TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
	rotation.travel = 0;
}

void rotation_start(void) {
	if ((rotation_is_min() & !rotation.direction) || (rotation_is_max() & rotation.direction)) {
		return;
	}
	
	//uint16_t OCR_START = rotation_freq_to_ocr_tick(rotation.start_freq);
	//uint16_t OCR_WORK = rotation_freq_to_ocr_tick(rotation.work_freq);
	
	rotation.OCR_value = rotation.OCR_start;
	
	rotation.half_steps	= 0;
	rotation.steps_acceleration = (uint16_t)(abs(rotation.OCR_start - rotation.OCR_work) / ROTATION_ACCELERATION);
	rotation.steps_deceleration = (uint16_t)(abs(rotation.OCR_work - rotation.OCR_finish) / ROTATION_DECELERATION);
	
	if (rotation.steps_acceleration > rotation.steps_count / 2) {
		rotation.steps_acceleration = rotation.steps_count / 2;
	}
	if (rotation.steps_deceleration > rotation.steps_count / 2) {
		rotation.steps_deceleration = rotation.steps_count / 2;
	}
	
	//rotation.by_step	= by_step;
	//rotation.enabled	= 1;

	ROTATION_OCR = (uint16_t)rotation.OCR_value;
	
	TCCR1B |= (0<<CS12) | (1<<CS11) | (0<<CS10);
	rotation.ready = 0;
	rotation.travel = 1;
}

uint8_t rotation_is_max(void) {
	return ((ROTATION_INT_PORT & (1<<ROTATION_INT_MAX_PIN)) == 0);
}

uint8_t rotation_is_min(void) {
	return ((ROTATION_INT_PORT & (1<<ROTATION_INT_MIN_PIN)) == 0);
}

void rotation_set_dir(uint8_t direction) {
	rotation.direction = direction;
	if (direction) {
		ROTATION_DIR_PORT &= ~(1<<ROTATION_DIR_PIN);
	} else {
		ROTATION_DIR_PORT |= (1<<ROTATION_DIR_PIN);
	}
}

void rotation_speed_change(void) {
	// Ускорение
	if (rotation.acceleration_enabled) {
		if (rotation.OCR_value > rotation.OCR_work) {
			rotation.OCR_value -= ROTATION_ACCELERATION;
			ROTATION_OCR = (uint16_t)rotation.OCR_value;
		}
	}
	
	// Замедление
	if (rotation.deceleration_enabled) {
		if (rotation.OCR_value < rotation.OCR_finish) {
			rotation.OCR_value += ROTATION_DECELERATION;
			ROTATION_OCR = (uint16_t)rotation.OCR_value;
		}
	}
}

void rotation_acceleration_enable(void) {
	rotation_deceleration_disable();
	rotation.acceleration_enabled = 1;
}

void rotation_acceleration_disable(void) {
	rotation.acceleration_enabled = 0;
}

void rotation_deceleration_enable(void) {
	rotation_acceleration_disable();
	rotation.deceleration_enabled = 1;
}

void rotation_deceleration_disable(void) {
	rotation.deceleration_enabled = 0;
}

void rotation_led_min(void) {
	ROTATION_LED_PORT |= (1<<ROTATION_LED_MIN_PIN);
}

void rotation_led_max(void) {
	ROTATION_LED_PORT |= (1<<ROTATION_LED_MAX_PIN);
}

void rotation_led_disable(void) {
	ROTATION_LED_PORT &= ~((1<<ROTATION_LED_MIN_PIN) | (1<<ROTATION_LED_MAX_PIN));
}

uint8_t rotation_check_ready(void) {
	if (!rotation.ready) {
		ROTATION_READY_PORT |= (1<<ROTATION_READY_PIN);
	} else {
		ROTATION_READY_PORT &= ~(1<<ROTATION_READY_PIN);
	}
	return rotation.ready;
}