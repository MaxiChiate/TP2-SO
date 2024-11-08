//list_int.c

#include <Collections/list_int.h>


list_t list_int_init()  {

    return list_init(sizeof(int64_t));
}



void list_int_add(list_t my_list, int64_t data) {

      list_add(my_list, &data);
}



uint32_t list_int_get_size(list_t my_list)  {

    return list_get_size(my_list);
}



int64_t list_int_get_head(list_t my_list) {

    int64_t * data_ptr = (int64_t *) list_get_head(my_list);
    
    return not_null(data_ptr) ? *data_ptr : INT64_MAX;
}


bool list_int_is_empty(list_t my_list)  {

    return list_is_empty(my_list);
}

void list_int_delete_first(list_t my_list) {
  
    list_delete_first(my_list);
}

void list_int_free(list_t my_list) {
    
    list_free(my_list);
}