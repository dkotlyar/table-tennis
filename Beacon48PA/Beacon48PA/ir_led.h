/*
 * ir_led.h
 *
 * Created: 13.01.2017 11:21:50
 *  Author: Dmitry
 */ 


#ifndef IR_LED_H_
#define IR_LED_H_

#define IRLED_FREQ	36000

#define IRLED_PORT	PORTD
#define IRLED_DDR	DDRD
#define IRLED_PIN	4

void irled_init(void);
void timer2_init(void);
void irled_enable(void);
void irled_disable(void);


#endif /* IR_LED_H_ */