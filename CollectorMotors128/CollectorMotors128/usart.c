/*
 * usart.c
 *
 * Created: 24.03.2016
 * Author: Dmitry Kotlyar
 */

#include "main.h"
#include "usart.h"
#include "message.h"
#include "led.h"
#include <string.h>

usart_t usart;

void usart_receive(uint8_t data);
void increment_index(uint8_t * index);
uint8_t usart1_ready(void);
void usart1_busy_off(void);
void usart1_busy_on(void);
uint8_t usart1_is_busy(void);

ISR (USART1_RX_vect) {
	usart_receive(UDR1);
}

void usart_receive(uint8_t data) {
	static uint8_t status = WAIT_START;
	static uint8_t size = 0;
	static uint8_t index = 0;
	static uint8_t checksum = 0;
	
	if (status == WAIT_START && data == START_TRANSMIT) {
		status = WAIT_SIZE;
	} else if (status == WAIT_SIZE) {
		size = data;
		if (size > USART_BUFFER_SIZE) {
			status = WAIT_START;
			return;
		}
		
		status = RECEIVE;
		index = 0;
		checksum = 0;
	} else if (status == RECEIVE) {
		usart.receive.element[usart.receive.index_add].buffer[index] = data;
		checksum += data;
		index++;
		if (index == size) {
			status = WAIT_CHECKSUM;	
		}
	} else if (status == WAIT_CHECKSUM) {
		if (checksum == data) {// проверка контрольной суммы
			usart.receive.element[usart.receive.index_add].length = size;
			increment_index(&usart.receive.index_add);
		}
		status = WAIT_START;
	}
}

void usart1_init(void) {
	// USART initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: On
	// USART Transmitter: On
	// USART Mode: Asynchronous
	// USART Baud Rate: 9600
	UCSR1B = (1<<RXCIE1) |  // Разрешить прерывание по приёму данных
			 (1<<RXEN1) |   // Разрешить приём данных по USART
			 (1<<TXEN1) |   // Разрешить отправку данных по USART
			 (0<<UCSZ12);	// Формат посылки
	UCSR1C = (0<<UPM11) | (0<<UPM10) | // Проверка паритета отключена
			 (0<<USBS1) | // 1 стоповый бит
			 (1<<UCSZ11) | (1<<UCSZ10); // 8 бит передаваемых данных

	UBRR1H = (USART_SPEED >> 8) & 0xFF;
	UBRR1L = USART_SPEED & 0xFF;
	
	usart1_busy_off();
}


uint8_t usart1_ready(void) {
	// ожидание доступности USART для передачи
	return !((UCSR1A & (1<<UDRE1)) == 0);
}

void usart1_busy_off(void) {
	USART_BUSY_OUT &= ~(1<<USART_BUSY_PIN);
	USART_BUSY_DDR &= ~(1<<USART_BUSY_PIN);
}

void usart1_busy_on(void) {
	USART_BUSY_DDR |= (1<<USART_BUSY_PIN);
	USART_BUSY_OUT |= (1<<USART_BUSY_PIN);
}

uint8_t usart1_is_busy(void) {
	if (USART_BUSY_IN & (1<<USART_BUSY_PIN))
		return 1;
	else
		return 0;
}


void increment_index(uint8_t * index) {
	(*index)++;
	if (*index >= USART_BUFFER_COUNT) {
		*index = 0;
	}
}

void usart_add_transmit(uint8_t * data, uint8_t length) {
	usart.transmit.element[usart.transmit.index_add].length = length;
	memcpy(usart.transmit.element[usart.transmit.index_add].buffer, data, length);
	increment_index(&usart.transmit.index_add);
}

void usart_remove_transmit(uint8_t cmd) {
	while (usart.transmit.index_get != usart.transmit.index_add && usart.transmit.element[usart.transmit.index_get].buffer[0] == cmd) {
		increment_index(&usart.transmit.index_get);
	}
}

void usart_handle_transmit(void) {
	static uint8_t status = WAIT_START;
	static uint8_t index = 0;
	static uint8_t checksum = 0;
	
	if (!usart1_ready()) {
		return;
	}
	
	if (status == WAIT_START) { // начало передачи новой посылки
		if (usart.transmit.index_get == usart.transmit.index_add)
			return;
		//if (usart1_is_busy()) {
			//return;
		//}
		//usart1_busy_on();
		index = 0;
		checksum = 0;
		USART_DATA = START_TRANSMIT;
		status = WAIT_SIZE;
	} else if (status == WAIT_SIZE) {
		USART_DATA = usart.transmit.element[usart.transmit.index_get].length;
		status = TRANSMIT;
	} else if (status == TRANSMIT) {
		uint8_t data = usart.transmit.element[usart.transmit.index_get].buffer[index];
		USART_DATA = data;
		checksum += data;
		index++;
		if (index == usart.transmit.element[usart.transmit.index_get].length) {
			status = WAIT_CHECKSUM;
		}
	} else if (status == WAIT_CHECKSUM) {
		USART_DATA = checksum;
		status = WAIT_START;
		increment_index(&usart.transmit.index_get);
		//usart1_busy_off();
	}
}

void usart_handle_receive(void) {
	if (usart.receive.index_get != usart.receive.index_add) {
		handle_message(usart.receive.element[usart.receive.index_get].buffer, 
			usart.receive.element[usart.receive.index_get].length);
		increment_index(&usart.receive.index_get);
	}
}