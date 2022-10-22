/*
 * tsop.c
 *
 * Created: 13.01.2017 11:00:53
 *  Author: Dmitry
 */ 

#include "main.h"
#include "tsop.h"
#include "led.h"

//uint8_t tsop1, tsop2;

//ISR (INT6_vect) {
	////tsop1 = (TSOP_PORT & (1<<TSOP_PIN1)) >> TSOP_PIN1;
	//tsop1 = 1;
//}
//
//ISR (INT7_vect) {
	////tsop2 = (TSOP_PORT & (1<<TSOP_PIN2)) >> TSOP_PIN2;
	//tsop2 = 1;
//}

void tsop_init(void) {
	TSOP_DDR		&= ~((1<<TSOP_PIN1) | (1<<TSOP_PIN2));
	//TSOP_EN_DDR		|= (1<<TSOP_EN_PIN);
	TSOP_LED_DDR	|= ((1<<TSOP_LED_PIN1) | (1<<TSOP_LED_PIN2));
	TSOP_EXT_DDR	|= ((1<<TSOP_EXT_PIN1) | (1<<TSOP_EXT_PIN2));
	
	//// External Interrupt(s) initialization
	//// INT6: On
	//// INT6 Mode: Falling Edge
	//// INT7: On
	//// INT7 Mode: Falling Edge
	//EICRB	|= (1<<ISC71) | (0<<ISC70) | (1<<ISC61);
	//EIMSK	|= (1<<INT7) | (1<<INT6);
	//EIFR	|= (1<<INTF7) | (1<<INTF6);
}

//void tsop_enable(void) {
	//TSOP_EN_PORT |= (1<<TSOP_EN_PIN);
//}
//
//void tsop_disable(void) {
	//TSOP_EN_PORT &= ~(1<<TSOP_EN_PIN);
//}

void tsop_reset(void) {
	//tsop1 = 0;
	//tsop2 = 0;
}

uint8_t tsop_is_active(void) {
	uint8_t tsop1 = !((TSOP_INPUT & (1<<TSOP_PIN1))>>TSOP_PIN1);
	uint8_t tsop2 = !((TSOP_INPUT & (1<<TSOP_PIN2))>>TSOP_PIN2);
	
	TSOP_LED_PORT &= ~((1<<TSOP_LED_PIN1) | (1<<TSOP_LED_PIN2));
	TSOP_LED_PORT |= (tsop1<<TSOP_LED_PIN1);
	TSOP_LED_PORT |= (tsop2<<TSOP_LED_PIN2);
	
	uint8_t status = tsop1 || tsop2;

	TSOP_EXT_PORT &= ~((1<<TSOP_EXT_PIN1) | (1<<TSOP_EXT_PIN2));
	TSOP_EXT_PORT |= (status<<TSOP_EXT_PIN1);
	TSOP_EXT_PORT |= (status<<TSOP_EXT_PIN2);

	return status;
}