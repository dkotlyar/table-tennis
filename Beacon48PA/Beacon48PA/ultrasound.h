/*
 * ultrasound.h
 *
 * Created: 13.01.2017 11:26:30
 *  Author: Dmitry
 */ 


#ifndef ULTRASOUND_H_
#define ULTRASOUND_H_

#define US_FREQ		40000

#define US_DEFAULT_TIME 100000

#define US_EN_PORT	PORTB
#define US_EN_DDR	DDRB
#define US_EN_PIN	0

#define US_SIG_PORT	PORTB
#define US_SIG_DDR	DDRB
#define US_SIG_PIN1	1
#define US_SIG_PIN2	2

#define US_LED_PORT	PORTC
#define US_LED_DDR	DDRC
#define US_LED_PIN	0

void us_init(void);
void timer1_init(void);

void us_enable(void);
void us_disable(void);

void us_trig(uint16_t delay_micro);
void us_trig_default(void);

#endif /* ULTRASOUND_H_ */