/*
 * ultrasonic.h
 *
 * Created: 19.08.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define ULTRASONIC_CM_CONST 58

#define ULTRASONIC_PRESCALE		1

#define ULTRASONIC_PORT				PORTE
#define ULTRASONIC_DDR				DDRE
#define ULTRASONIC_PIN				PINE
#define ULTRASONIC_TRIG				2
#define ULTRASONIC_ECHO				5

void ultrasonic_init(void);
void timer2_init(void);

void ultrasonic_enable(void);
void ultrasonic_disable(void);

void ultrasonic_trig(void);
uint16_t ultrasonic_get_distance(void);

#endif /* ULTRASONIC_H_ */