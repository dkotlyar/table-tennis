/*
 * ir_led.h
 *
 * Created: 13.01.2017 11:21:50
 *  Author: Dmitry
 */ 


#ifndef IR_LED_H_
#define IR_LED_H_

#define IRLED_FREQ			36000
#define IRLED_FREQ_LEFT		56000
#define IRLED_FREQ_RIGHT	33000

#define IRLED_DDR	DDRB
#define IRLED_PORT	PORTB
#define IRLED_PIN	7

#define IR_EXT_DDR		DDRC
#define IR_EXT_PORT		PORTC
#define IR_EXT_INPUT	PINC
#define IR_EXT_PIN		3

void irled_init(void);
void timer2_init(void);
void irled_enable(uint16_t irled_freq);
void irled_disable(void);
void irled_check(void);

#endif /* IR_LED_H_ */