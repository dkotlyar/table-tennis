/*
 * led.c
 *
 * Created: 25.03.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "led.h"

void led_init(void) {
	LED_DDR |= (1<<LED_PIN);
}

void led_enable(void) {
	LED_PORT |= (1<<LED_PIN);
}

void led_disable(void) {
	LED_PORT &= ~(1<<LED_PIN);
}

void led_blink(uint8_t cnt) {
	led_disable();
	for (int i = 0; i < cnt; i++) {
		_delay_ms(200);
		led_enable();
		_delay_ms(200);
		led_disable();
	}
}
