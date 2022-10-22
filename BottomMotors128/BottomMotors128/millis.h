/*
 * millis.h
 *
 * Created: 25.03.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef MILLIS_H_
#define MILLIS_H_

#define MILLIS_vect TIMER0_COMP_vect
#define MILLIS_TIMER_FREQ 1000

void timer0_init(void);

uint32_t millis(void);
uint64_t micros(void);

#endif /* MILLIS_H_ */