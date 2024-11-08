//list_int.h
#ifndef __LIST_INT_H
#define __LIST_INT_H

    #include <Collections/list.h>

    list_t list_int_init();

    void list_int_add(list_t my_list, int64_t data);

    uint32_t list_int_get_size(list_t my_list);

    int64_t list_int_get_head(list_t my_list);

    bool list_int_is_empty(list_t my_list);

    void list_int_delete_first(list_t my_list);

    void list_int_free(list_t my_list);
    
#endif