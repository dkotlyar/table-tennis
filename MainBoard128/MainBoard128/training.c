/*
 * training.c
 *
 * Created: 8.08.2016
 * Author: Dmitry Kotlyar
 */

#include "main.h"
#include "training.h"

#include "led.h"
#include "cmd.h"
#include "message.h"
#include "motor.h"

struct {
	position_t positions[60];
	uint8_t positions_count;
	uint8_t	repeat_count;
	uint8_t index_position;
	uint8_t index_repeat;
	int16_t current_delay;
	int16_t over_delay;
	int16_t delay_ms;
} program;

position_t current_position;

uint8_t power_mode;
uint8_t program_status;
//uint8_t lock_next_step;
//uint8_t lock_training_tick;

uint16_t ball_speed_ticks;
uint16_t ball_speed;

ISR (TIMER1_OVF_vect) {
	ball_speed = 0;
	cymemotr_timer_disable();
}

ISR (INT4_vect) {
	uint16_t fly_time = TCNT1 / 2; // us
	cymemotr_timer_disable();

	if (fly_time > 0) {
		ball_speed = (uint16_t)(((uint32_t)SHOT_SPEED_DISTANCE * 1000000UL) / fly_time); // мм/с
	}

	uint8_t data[sizeof(uint16_t)+1];
	*data = BALL_SPEED;
	memcpy(data+1, &ball_speed, sizeof(uint16_t));
	transmit_to0(data, sizeof(uint16_t)+1);
}

ISR (INT5_vect) {
	TCNT1 = 0;
	cymemotr_timer_enable();

	if (check_full_ready() || power_mode == HAND_MODE) {
		feed_disable();
		next_step();
	}
}

//#ifdef USE_INT6_INT7
	//ISR (INT6_vect) {
	//}
//
	//ISR (INT7_vect) {
	//}
//#endif

void training_init(void) {
	READY_DDR &= ~((1<<READY_LIFT) | (1<<READY_ROTATION) | (1<<READY_INCLINE) | (1<<READY_ROLLROTATION) | (1<<READY_TOPROLL) | (1<<READY_BOTTOMROLL));
	READY_LED_DDR |= (1<<READY_LIFT) | (1<<READY_ROTATION) | (1<<READY_INCLINE) | (1<<READY_ROLLROTATION) |	(1<<READY_TOPROLL) | (1<<READY_BOTTOMROLL);

	EICRB	|=	(1<<ISC51) | (1<<ISC50) | 
				(1<<ISC41) | (1<<ISC40);
	EIMSK	|= (1<<INT5) | (1<<INT4);
	EIFR	|= (1<<INTF5) | (1<<INTF4);

	//#ifdef USE_INT6_INT7
		//EICRB	|=	(1<<ISC71) | (0<<ISC70) |
					//(1<<ISC61) | (0<<ISC60);
		//EIMSK	|= (1<<INT7) | (1<<INT6);
		//EIFR	|= (1<<INTF7) | (1<<INTF6);
	//#endif
	
	SHOT_LED_DDR |= (1<<SHOT_LED_A) | (1<<SHOT_LED_B) | (1<<SHOT_LED_C);
	SHOT_LED_PORT |= (1<<SHOT_LED_A) | (1<<SHOT_LED_B) | (1<<SHOT_LED_C);

	power_mode = HAND_MODE;
	program_status = PROGRAM_STOPPED;

	timer1_init();
}

// Частотомер
void timer1_init(void) {
	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 2000,000 kHz
	// Mode: Normal top=0xFFFF
	// OC1A output: Disconnected
	// OC1B output: Disconnected
	// OC1C output: Disconnected
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer Period: 32,768 ms
	// Timer1 Overflow Interrupt: On
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off
	TCCR1A	=	(0<<COM1A1) | (0<<COM1A0) | 
				(0<<COM1B1) | (0<<COM1B0) | 
				(0<<COM1C1) | (0<<COM1C0) | 
				(0<<WGM11) | (0<<WGM10);
	TCCR1B	=	(0<<ICNC1) | (0<<ICES1) | 
				(0<<WGM13) | (0<<WGM12) | 
				(0<<CS12) | (0<<CS11) | (0<<CS10);
	TCNT1	= 0x00;
	ICR1	= 0x00;
	OCR1A	= 0x00;
	OCR1B	= 0x00;
	OCR1C	= 0x00;

	TIMSK	|= (1<<TOIE1);
}

void cymemotr_timer_enable(void) {
	TCCR1B |= (0<<CS12) | (1<<CS11) | (0<<CS10);
}

void cymemotr_timer_disable(void) {
	TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
}

void set_mode(uint8_t _mode) {
	if (power_mode == _mode) {
		return;
	}

	stop_program();
	disable_collector_motors();
	power_mode = _mode;
}

uint8_t get_mode(void) {
	return power_mode;
}

uint8_t check_ready(void) {
	READY_LED_PORT = 0;

	if ((READY_PIN & (1<<READY_LIFT)) == 0) {
		READY_LED_PORT |= (1<<READY_LIFT);
	} else if (program.over_delay > 0 && !(program.over_delay % OVER_DELAY_MS)) {
		set_lift(current_position.lift);
	}

	if ((READY_PIN & (1<<READY_ROTATION)) == 0) {
		READY_LED_PORT |= (1<<READY_ROTATION);
	} else if (program.over_delay > 0 && !(program.over_delay % OVER_DELAY_MS)) {
		set_rotation(current_position.rotation);
	}

	if ((READY_PIN & (1<<READY_INCLINE)) == 0) {
		READY_LED_PORT |= (1<<READY_INCLINE);
	} else if (program.over_delay > 0 && !(program.over_delay % OVER_DELAY_MS)) {
		set_incline(current_position.incline);
	}

	if ((READY_PIN & (1<<READY_ROLLROTATION)) == 0) {
		READY_LED_PORT |= (1<<READY_ROLLROTATION);
	} else if (program.over_delay > 0 && !(program.over_delay % OVER_DELAY_MS)) {
		set_rollrotation(current_position.rollrotation);
	}

	if ((READY_PIN & (1<<READY_TOPROLL)) == 0) {
		READY_LED_PORT |= (1<<READY_TOPROLL);
	} else if (program.over_delay > 0 && !(program.over_delay % OVER_DELAY_MS)) {
		set_top_speed(current_position.toproll);
	}

	if ((READY_PIN & (1<<READY_BOTTOMROLL)) == 0) {
		READY_LED_PORT |= (1<<READY_BOTTOMROLL);
	} else if (program.over_delay > 0 && !(program.over_delay % OVER_DELAY_MS)) {
		set_bottom_speed(current_position.bottomroll);
	}

	uint8_t ready = !(READY_PIN & ((1<<READY_LIFT) | (1<<READY_ROTATION) | (1<<READY_INCLINE) | (1<<READY_ROLLROTATION) | (1<<READY_TOPROLL) | (1<<READY_BOTTOMROLL)));

	return ready;
}

uint8_t check_full_ready(void) {
	uint8_t delay_ready = program.current_delay >= program.delay_ms;

	if (delay_ready) {
		led_enable();
	} else {
		led_disable();
	}

	return check_ready() && delay_ready;
}


void disable_collector_motors(void) {
	 uint8_t data[1];
	 *data = TR_DISABLE;
	 transmit_to1(data, 1);
	 *data = BR_DISABLE;
	 transmit_to1(data, 1);
}
 

void start_program(void) {
	if (program_status == PROGRAM_STARTED) {
		return;
	}
	if (power_mode == HAND_MODE) {
		return;
	}

	if (program_status == PROGRAM_STOPPED) {
		uint8_t data[2] = { ENABLE_MOTOR, 0 };
		transmit_to1(data, 2);

		program.index_position = 0;
		program.index_repeat = 0;

		//lock_next_step = 0;
		program_status = PROGRAM_STARTED;
		next_step();
	} else if (program_status == PROGRAM_PAUSED) {
		uint8_t data[sizeof(uint16_t)+1];
		*data = TR_DISABLE;
		memcpy(data+1, &current_position.toproll, sizeof(uint16_t));
		transmit_to1(data, sizeof(uint16_t)+1);
		
		*data = BR_DISABLE;
		memcpy(data+1, &current_position.bottomroll, sizeof(uint16_t));
		transmit_to1(data, sizeof(uint16_t)+1);

		program_status = PROGRAM_STARTED;
	}

}

void stop_program(void) {
	if (program_status == PROGRAM_STOPPED) {
		return;
	}

	program_status = PROGRAM_STOPPED;
	program.over_delay = 0;
	program.delay_ms = 0;
	program.current_delay = 0;

	feed_disable();
	disable_collector_motors();
	
	//uint8_t data[2] = { DISABLE_MOTOR, 0 };
	//transmit_to1(data, 2);

	_delay_ms(500);

	set_lift(800);
	set_rotation(0);
	set_incline(0);
	set_rollrotation(0);
}

void pause_program(void) {
	if (program_status == PROGRAM_STARTED) {
		program_status = PROGRAM_PAUSED;
		disable_collector_motors();
		feed_disable();
	}
}

void next_step(void) {

	//if (lock_next_step) {
		//return;
	//}

	if (program_status != PROGRAM_STARTED || power_mode == HAND_MODE) {
		return;
	}

	if (program.index_repeat >= program.repeat_count) {
		program.index_repeat = 0;
		stop_program();

		uint8_t data[1] = { STOP_PROGRAM };
		transmit_to0(data, 1);

		return;
	}

	memcpy(&current_position, &program.positions[program.index_position], sizeof(position_t));

	set_lift(current_position.lift);
	set_rotation(current_position.rotation);
	set_incline(current_position.incline);
	set_rollrotation(current_position.rollrotation);
	set_top_speed(current_position.toproll);
	set_bottom_speed(current_position.bottomroll);

	program.current_delay = 0;
	program.over_delay = 0;
	program.delay_ms = current_position.delay_ms;

	// уменьшаем задержку, т.к. шнеку нужно время прокрутиться
	if (program.delay_ms > STOK_DELAY_MS) {
		program.delay_ms -= STOK_DELAY_MS;
	} else {
		program.delay_ms = 0;
	}

	// задержка не может быть слишком маленькой, иначе устройства не успеют сбросить флаг готовности
	if (program.delay_ms < MINIMUM_DELAY_MS) {
		program.delay_ms = MINIMUM_DELAY_MS;
	}

	if (++program.index_position >= program.positions_count) {
		program.index_position = 0;
		program.index_repeat++;
	}

	//lock_training_tick = 0;
}

void training_tick(void) {

	//if (lock_training_tick) {
		//return;
	//}
	
	//led_disable();
	if (program_status != PROGRAM_STARTED || power_mode == HAND_MODE) {
		//led_blink(2);
		return;
	}
	//
	//if (program.current_delay < program.delay_ms) {
		//program.current_delay++;
		//} else {
		////led_enable();
		//if (check_ready() /*&& lock_next_step*/) {
			//lock_next_step = 0;
			////lock_training_tick = 1;
			//feed_enable();
			//} else {
			//program.over_delay++;
//
			//if (program.over_delay > MAXIMUM_DELAY_MS) {
				//lock_next_step = 0;
				//next_step();
			//}
		//}
	//}

	if (program.current_delay < program.delay_ms) {
		program.current_delay += 50;
	}

	if (check_full_ready()) {
		feed_enable();
	} else {
		program.over_delay += 50;
		if (!check_ready() && program.over_delay > MAXIMUM_DELAY_MS) {
			next_step();
		}
	}
}


void set_repeat_count(uint8_t repeat_count) {
	program.repeat_count = repeat_count;
}

void clear_program(void) {
	program.positions_count = 0;
	program.index_position = 0;
}

void append_shot(position_t pos) {
	memcpy(&program.positions[program.positions_count], &pos, sizeof(position_t));
	program.positions_count++;
}

void set_lift(int16_t lift) {
	uint8_t data[5];
	*data = LIFT;
	memcpy(data+1, &lift, sizeof(int16_t));
	transmit_to1(data, sizeof(int16_t)+1);
}

void set_rotation(int16_t rotation) {
	uint8_t data[5];
	*data = ROTATION;
	memcpy(data+1, &rotation, sizeof(int16_t));
	transmit_to1(data, sizeof(int16_t)+1);
}

void set_incline(int8_t incline) {
	uint8_t data[5];
	*data = INCLINE;
	memcpy(data+1, &incline, sizeof(int8_t));
	transmit_to1(data, sizeof(int8_t)+1);
}

void set_rollrotation(int16_t rollrotation) {
	uint8_t data[5];
	*data = ROLLROTATION;
	memcpy(data+1, &rollrotation, sizeof(int16_t));
	transmit_to1(data, sizeof(int16_t)+1);
}

void set_top_speed(int16_t toproll) {
	uint8_t data[5];
	*data = TR_SPEED;
	memcpy(data+1, &toproll, sizeof(int16_t));
	transmit_to1(data, sizeof(int16_t)+1);
}

void set_bottom_speed(int16_t bottomroll) {
	uint8_t data[5];
	*data = BR_SPEED;
	memcpy(data+1, &bottomroll, sizeof(int16_t));
	transmit_to1(data, sizeof(int16_t)+1);
}