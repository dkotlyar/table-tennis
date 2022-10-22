/*
 * queue.c
 *
 * Created: 24.03.2016 11:27:53
 *  Author: Dmitriy
 */ 

#include "main.h"
#include "queue.h"

// 306 ����-����

// ��������� ������� � ����� �������
void queue_push(queue_t * queue, byte * data, byte length) {
	list_t * node = (list_t*)malloc(sizeof(list_t));
	node->length = length;
	node->data = (byte*)malloc(sizeof(byte)*length);
	memcpy(node->data, data, length);

	if (queue->size == 0) {
		queue->head = node;
		queue->tail = node;
	} else {
		node->next = queue->head;
		queue->head->prev = node;	
		queue->head = node;
	}

	queue->size++;
}

// ��������� ������� � ������ �������
void queue_enqueue(queue_t * queue, byte * data, byte length) {
	list_t * node = (list_t*)malloc(sizeof(list_t));
	node->length = length;
	node->data = (byte*)malloc(sizeof(byte)*length);
	memcpy(node->data, data, length);

	if (queue->size == 0) {
		queue->head = node;
		queue->tail = node;
	} else {
		node->prev = queue->tail;
		queue->tail->next = node;
		queue->tail = node;
	}

	queue->size++;
}

// ��������� ������� �� ������ ������� � ������� ���
byte * queue_pop(queue_t * queue, byte * length) {
	if (queue->head != NULL) {
		byte * data = queue_peek(queue, length);
		queue_dequeue(queue);			
		return data;
	} else {
		*length = 0;
		return NULL;
	}
}

// ���������� ������� �� ������ �������, �� �� ������� ���
byte * queue_peek(queue_t * queue, byte * length) {
	if (queue->head != NULL) {
		*length = queue->head->length;
		byte * data = (byte*)malloc(sizeof(byte) * *length);
		memcpy(data, queue->head->data, *length);		
		return data;
	} else {
		*length = 0;
		return NULL;
	}
}

// ������� ������� �� ������ �������
void queue_dequeue(queue_t * queue) {
	queue->head = queue->head->next;
	free(queue->head->prev->data);
	free(queue->head->prev);
	queue->head->prev = NULL;
	
	queue->size--;
}