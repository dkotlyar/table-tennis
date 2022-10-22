/*
 * infrared.h
 *
 * Created: 22.08.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef INFRARED_H_
#define INFRARED_H_

#define INFRARED_PORT	PORTF
#define INFRARED_DDR	DDRF
#define INFRARED_PIN	0

void infrared_init(void);
uint16_t infrared_get_adc(void);
uint16_t infrared_get_distance(void);

#endif /* INFRARED_H_ */