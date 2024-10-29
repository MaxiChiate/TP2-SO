#ifndef QUEUE_H
#define QUEUE_H

#include <Collections/list.h>
#include <bool.h>

typedef list_t queue_t;

queue_t queue_init();

void enqueue(queue_t q, void *data);

void *dequeue(queue_t q);

void free_queue(queue_t q);

void queue_print(queue_t q);

bool queue_is_empty(queue_t q);

#endif // QUEUE_H