#include <Collections/queue.h>

queue_t queue_init()    {

    return list_int_init();
}

void enqueue(queue_t q, int64_t data) {

    list_int_add(q, data);
}

int64_t dequeue(queue_t q) {

    int64_t to_return = list_int_get_head(q);
    list_int_delete_first(q);
    return to_return;
}

void free_queue(queue_t q) {

    list_int_free(q);
}

bool queue_is_empty(queue_t q) {
    
    return list_int_is_empty(q);
}