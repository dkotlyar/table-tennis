/*
 * main.h
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL

#define INCLINE_CORRECTION_ADDR			0x0030
#define ROLLROTATION_CORRECTION_ADDR	0x0031

 #define SHOW_READY_ON_LED

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdlib.h>
#include <string.h>

#endif /* MAIN_H_ */