/*
 * tsop.h
 *
 * Created: 13.01.2017 11:01:00
 *  Author: Dmitry
 */ 


#ifndef TSOP_H_
#define TSOP_H_

#define TSOP_EN_PORT	PORTC
#define TSOP_EN_DDR		DDRC
#define TSOP_EN_PIN		1

#define TSOP_PORT	PORTD
#define TSOP_DDR	DDRD
#define TSOP_INPUT	PIND
#define TSOP_PIN1	2
#define TSOP_PIN2	3

#define TSOP_LED_PORT	PORTC
#define TSOP_LED_DDR	DDRC
#define TSOP_LED_PIN1	2
#define TSOP_LED_PIN2	3

void tsop_init(void);
void tsop_enable(void);
void tsop_disable(void);
void tsop_reset(void);
uint8_t tsop_is_active(void);

#endif /* TSOP_H_ */