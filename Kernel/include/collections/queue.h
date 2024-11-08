#ifndef QUEUE_H
#define QUEUE_H

#include <Collections/list_int.h>

typedef list_t queue_t;

queue_t queue_init();

void enqueue(queue_t q, int64_t data);

int64_t dequeue(queue_t q);

void free_queue(queue_t q);

void queue_print(queue_t q);

bool queue_is_empty(queue_t q);

#endif // QUEUE_H