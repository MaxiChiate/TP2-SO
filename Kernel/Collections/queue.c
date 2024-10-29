#include <Collections/queue.h>

queue_t queue_init() {
    return list_init();
}

void enqueue(queue_t q, void * data) {
    list_add(q, data);
}

void * dequeue(queue_t q) {
    void * to_return = list_get_head(q);
    delete_first(q);
    return to_return;
}

void free_queue(queue_t q) {
    list_mm_free(q);
}

bool queue_is_empty(queue_t q) {
    return list_get_size(q) == 0;
}