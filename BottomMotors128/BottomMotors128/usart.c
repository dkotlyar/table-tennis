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

usart_t usart0, usart1;

ISR (USART0_RX_vect) {
	usart_receive(get_usart0());
}

ISR (USART1_RX_vect) {
	usart_receive(get_usart1());
}

void usart_receive(usart_t * usart) {
	uint8_t data = *usart->DATA;
	
	if (usart->receive.status == WAIT_START && data == START_TRANSMIT) {
		usart->receive.status = WAIT_SIZE;
	} else if (usart->receive.status == WAIT_SIZE) {
		usart->receive.size = data;
		if (usart->receive.size > USART_BUFFER_SIZE) {
			usart->receive.status = WAIT_START;
			return;
		}
		
		usart->receive.status = RECEIVE;
		usart->receive.index = 0;
		usart->receive.checksum = 0;
	} else if (usart->receive.status == RECEIVE) {
		usart->receive.element[usart->receive.index_add].buffer[usart->receive.index] = data;
		usart->receive.checksum += data;
		usart->receive.index++;
		if (usart->receive.index == usart->receive.size) {
			usart->receive.status = WAIT_CHECKSUM;	
		}
	} else if (usart->receive.status == WAIT_CHECKSUM) {
		if (usart->receive.checksum == data) {// проверка контрольной суммы
			usart->receive.element[usart->receive.index_add].length = usart->receive.size;
			increment_index(&usart->receive.index_add);
		}
		usart->receive.status = WAIT_START;
	}
}

void usart0_init(void) {
	// USART initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: On
	// USART Transmitter: On
	// USART Mode: Asynchronous
	// USART Baud Rate: 9600
	UCSR0B = (1<<RXCIE0) |  // Разрешить прерывание по приёму данных
			 (1<<RXEN0) |   // Разрешить приём данных по USART
			 (1<<TXEN0) |   // Разрешить отправку данных по USART
			 (0<<UCSZ02);	// Формат посылки
	UCSR0C = (0<<UPM01) | (0<<UPM00) | // Проверка паритета отключена
			 (0<<USBS0) | // 1 стоповый бит
			 (1<<UCSZ01) | (1<<UCSZ00); // 8 бит передаваемых данных

	UBRR0H = (USART_SPEED >> 8) & 0xFF;
	UBRR0L = USART_SPEED & 0xFF;
	
	usart_t * usart = get_usart0();
	usart->receive_handler = &handle_from0;
	usart->DATA = &UDR0;
	usart_init(usart);
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
	
	usart_t * usart = get_usart1();
	usart->receive_handler = handle_from1;
	usart->DATA = &UDR1;
	usart_init(usart);
}

void usart_init(usart_t * usart) {
	usart->receive.status = WAIT_START;
	usart->transmit.status = WAIT_START;
	//usart_busy_off(usart);
}


void increment_index(uint8_t * index) {
	(*index)++;
	if (*index >= USART_BUFFER_COUNT) {
		*index = 0;
	}
}

uint8_t usart_is_ready(usart_t * usart) {
	// ожидание доступности USART для передачи
	if (usart == get_usart0()) {
		return !((UCSR0A & (1<<UDRE0)) == 0);
	} else {
		return !((UCSR1A & (1<<UDRE1)) == 0);
	}
}

//void usart_busy_off(usart_t * usart) {
	//USART_BUSY_OUT &= ~(1<<USART_BUSY_PIN);
	//USART_BUSY_DDR &= ~(1<<USART_BUSY_PIN);
//}
//
//void usart_busy_on(usart_t * usart) {
	//USART_BUSY_DDR |= (1<<USART_BUSY_PIN);
	//USART_BUSY_OUT |= (1<<USART_BUSY_PIN);
//}
//
//uint8_t usart_is_busy(usart_t * usart) {
	//if (USART_BUSY_IN & (1<<USART_BUSY_PIN))
		//return 1;
	//else
		//return 0;
//}

void usart_print(usart_t * usart, char * str) {
	//while (1) {
	while (*str != 0) {
		while (!usart_is_ready(usart)) ;

		//while (((UCSR1A & (1<<UDRE1)) == 0)) ;

		*usart->DATA = *str;
		//UDR1 = *str;
		str++;
	}
	
	while (!usart_is_ready(usart)) ;
	*usart->DATA = '\n';
	while (!usart_is_ready(usart)) ;
	*usart->DATA = '\r';
}

void usart_add_transmit(usart_t * usart, uint8_t * data, uint8_t length) {
	usart->transmit.element[usart->transmit.index_add].length = length;
	memcpy(usart->transmit.element[usart->transmit.index_add].buffer, data, length);
	increment_index(&usart->transmit.index_add);
}

void usart_handle_transmit(usart_t * usart) {
	if (!usart_is_ready(usart)) {
		return;
	}
	
	if (usart->transmit.status == WAIT_START) { // начало передачи новой посылки
		if (usart->transmit.index_get == usart->transmit.index_add)
			return;
		//if (usart1_is_busy()) {
			//return;
		//}
		//usart1_busy_on();
		usart->transmit.index = 0;
		usart->transmit.checksum = 0;
		*usart->DATA = START_TRANSMIT;
		usart->transmit.status = WAIT_SIZE;
	} else if (usart->transmit.status == WAIT_SIZE) {
		*usart->DATA = usart->transmit.element[usart->transmit.index_get].length;
		usart->transmit.status = TRANSMIT;
	} else if (usart->transmit.status == TRANSMIT) {
		uint8_t data = usart->transmit.element[usart->transmit.index_get].buffer[usart->transmit.index];
		*usart->DATA = data;
		usart->transmit.checksum += data;
		usart->transmit.index++;
		if (usart->transmit.index == usart->transmit.element[usart->transmit.index_get].length) {
			usart->transmit.status = WAIT_CHECKSUM;
		}
	} else if (usart->transmit.status == WAIT_CHECKSUM) {
		*usart->DATA = usart->transmit.checksum;
		usart->transmit.status = WAIT_START;
		increment_index(&usart->transmit.index_get);
		//usart1_busy_off();
	}
}

void usart_handle_receive(usart_t * usart) {
	if (usart->receive.index_get != usart->receive.index_add) {
		usart->receive_handler(usart->receive.element[usart->receive.index_get].buffer, 
			usart->receive.element[usart->receive.index_get].length);
		increment_index(&usart->receive.index_get);
	}
}

usart_t * get_usart0(void) {
	return &usart0;
}

usart_t * get_usart1(void) {
	return &usart1;
}