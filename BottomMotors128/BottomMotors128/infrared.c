/*
 * infrared.c
 *
 * Created: 22.08.2016
 * Author: Dmitry Kotlyar
 */ 

#include "main.h"
#include "infrared.h"
#include "led.h"

void infrared_init(void) {
	
	INFRARED_DDR &= ~(1<<INFRARED_PIN);

	ADMUX |=	(0<<REFS1) | (1<<REFS0) |
				(0<<ADLAR) | // ����� ��������
				(INFRARED_PIN << MUX0);

	ADCSRA |=	(1<<ADEN) | // ���������� ������ ���
				(1<<ADSC) | // ������ ���
				(1<<ADFR) | // �������������� ����� ���
				(1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // ������������ ��� 128
}

uint16_t infrared_get_adc(void) {
	return ADC;
}

uint16_t infrared_get_distance(void) {
	float volts = infrared_get_distance() * 5 / 1024;
	float distance = 65 * pow(volts, -1.1);
	return (uint16_t)distance;
}