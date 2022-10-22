/*
 * led.h
 *
 * Created: 25.03.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef LED_H_
#define LED_H_

#define LED_PORT PORTC
#define LED_DDR DDRC
#define LED_PIN 6

void led_init(void);
void led_enable(void);
void led_disable(void);
void led_blink(uint8_t cnt);

#endif /* LED_H_ */