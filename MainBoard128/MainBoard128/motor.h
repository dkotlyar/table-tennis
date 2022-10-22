/*
 * motor.h
 *
 * Created: 27.06.2016 13:02:51
 *  Author: Dmitriy
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#define FEED_PWM_ENABLE

#define FEED_FREQ		2000
#define FEED_PWM		0.25
#define FEED_PRESCALE	64

#define FEED_PORT	PORTE
#define FEED_DDR	DDRE
#define FEED_PIN	3

void feed_init(void);
void timer3_init(void);
uint16_t feed_freq_to_ocr_ticks(uint16_t freq);

void feed_enable(void);
void feed_disable(void);

#endif /* MOTOR_H_ */