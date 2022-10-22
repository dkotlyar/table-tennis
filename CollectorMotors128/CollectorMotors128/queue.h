/*
 * queue.h
 *
 * Created: 24.03.2016 11:28:02
 *  Author: Dmitriy
 */ 


#ifndef QUEUE_H_
#define QUEUE_H_

void queue_enqueue(queue_t * queue, byte * data, byte length);
void queue_push(queue_t * queue, byte * data, byte length);
byte * queue_pop(queue_t * queue, byte * length);
byte * queue_peek(queue_t * queue, byte * length);
void queue_dequeue(queue_t * queue);

#endif /* QUEUE_H_ */