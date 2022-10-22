/*
 * main.h
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL

#define SHOW_READY_ON_LED
#define	SEND_SPEED
#define RISING_EDGE_INT

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

void send_speed(void);

#endif /* MAIN_H_ */