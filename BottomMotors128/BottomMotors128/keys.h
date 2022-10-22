/*
 * keys.h
 *
 * Created: 07.06.2016 13:58:57
 *  Author: Dmitriy
 */ 


#ifndef KEYS_H_
#define KEYS_H_

#define SHORT_DELAY		300
#define ONCELONG_DELAY	1000
#define MULTLONG_DELAY	1300

#define KEY_PORT	PINB
#define KEY_PUD		PORTB
#define KEY_DDR		DDRB
#define KEY_PIN		0

#define KEY_ZERO	(1<<KEY_PIN)

void key_init(void);
void key_handle(void);
uint16_t read_key_image(void);

void multiple_long_key_press(uint16_t key_image);
void once_long_key_press(uint16_t key_image);
void short_key_press(uint16_t key_image);

#endif /* KEYS_H_ */