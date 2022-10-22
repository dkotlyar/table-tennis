/*
 * tsop.c
 *
 * Created: 13.01.2017 11:00:53
 *  Author: Dmitry
 */ 

#include "main.h"
#include "tsop.h"

//uint8_t tsop1, tsop2;

//ISR (INT0_vect) {
	////tsop1 = (TSOP_INPUT & (1<<TSOP_PIN1)) >> TSOP_PIN1;
	//tsop1 = 1;
//}
//
//ISR (INT1_vect) {
	////tsop2 = (TSOP_INPUT & (1<<TSOP_PIN2)) >> TSOP_PIN2;
	//tsop2 = 1;
//}

void tsop_init(void) {
	TSOP_DDR		&= ~((1<<TSOP_PIN1) | (1<<TSOP_PIN2));
	TSOP_EN_DDR		|= (1<<TSOP_EN_PIN);
	TSOP_LED_DDR	|= ((1<<TSOP_LED_PIN1) | (1<<TSOP_LED_PIN2));

	//// External Interrupt(s) initialization
	//// INT0: On
	//// INT0 Mode: Falling Edge
	//// INT1: On
	//// INT1 Mode: Falling Edge
	//EICRA	= (1<<ISC11) | (0<<ISC10) | (1<<ISC01) | (0<<ISC00);
	//EIMSK	= (1<<INT1) | (1<<INT0);
	//EIFR	= (1<<INTF1) | (1<<INTF0);

	tsop_disable();
}

void tsop_enable(void) {
	TSOP_EN_PORT &= ~(1<<TSOP_EN_PIN);
}

void tsop_disable(void) {
	TSOP_EN_PORT |= (1<<TSOP_EN_PIN);
}

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
	return status;
}