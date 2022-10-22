/*
 * usart.h
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */ 


#ifndef USART_H_
#define USART_H_

#define USART_BAUD 9600
#define USART_SPEED (F_CPU / 16 / USART_BAUD - 1)
#define USART_BUFFER_SIZE 8
#define USART_BUFFER_COUNT 32

#define USART_RX_vect USART1_RX_vect
#define USART_DATA UDR1

#define USART_BUSY_DDR	DDRD
#define USART_BUSY_OUT	PORTD
#define USART_BUSY_IN	PIND
#define USART_BUSY_PIN	4

typedef struct __usart_buffer_element__ {
	uint8_t buffer[USART_BUFFER_SIZE];
	uint8_t length;
} usart_buffer_element_t;

typedef struct __usart_buffer__ {
	usart_buffer_element_t element[USART_BUFFER_COUNT];
	uint8_t index_add;
	uint8_t index_get;
} usart_buffer_t;

typedef struct __usart__ {
	usart_buffer_t transmit;
	usart_buffer_t receive;
} usart_t;

enum __usart_service__ {
	START_TRANSMIT = 0xF0,
	STOP_TRANSMIT = 0xF1
};

enum usart_status {
	WAIT_START,
	WAIT_SIZE,
	RECEIVE,
	TRANSMIT,
	WAIT_CHECKSUM,
};

void usart1_init(void);

void usart_add_receive(uint8_t length);
void usart_add_transmit(uint8_t * data, uint8_t length);

void usart_handle_transmit(void);
void usart_handle_receive(void);

#endif /* USART_H_ */