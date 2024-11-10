// buffer.c
#include <Collections/buffer.h>

struct buffer {

    char * data;
    int current_index;
    uint32_t size;
    bool is_standard;

//Cuantos procesos lo usan:
    int processes_referencing_it;
};

static inline bool is_null(buffer_t buffer);
static inline bool not_null(buffer_t buffer);


buffer_t buffer_init(uint32_t size, bool is_standard) {

    if(size == 0)   {

        return NULL;
    }

    buffer_t buffer = (buffer_t) mm_malloc(sizeof(*buffer));

    if (!buffer) {
    
        return NULL;
    }

    buffer->data = (char *) mm_malloc(size*sizeof(buffer->data[0]));

    if (!buffer->data) {
    
        mm_free(buffer);
        return NULL;
    }

    buffer->current_index = 0;
    buffer->size = size;
    buffer->is_standard = is_standard;
    buffer->processes_referencing_it = 1;

    return buffer;
}



void buffer_free(buffer_t buffer) {

    if (not_null(buffer)) {

        mm_free(buffer->data);
        mm_free(buffer);
    }
}



int buffer_write(buffer_t buffer, const char *src, int size)   {

    if(is_null(buffer) || src == NULL)  {
        
        return -1;
    }

    if ((buffer->current_index + size) > buffer->size) {

        size = buffer->size - buffer->current_index;
    }

    memcpy(buffer->data + buffer->current_index, src, size);
    
    buffer->current_index += size;

    return size;  
}



int buffer_read(buffer_t buffer, char *dest, int size) {

    if(is_null(buffer) || dest == NULL) {

        return -1;
    }

    if (size > buffer->current_index)   {

        size = buffer->current_index;
    }

    int bytes_read;

// Copia todo el size y frena si hay un EOF (tambien lo copia):
    for (bytes_read = 0; bytes_read < size && 
        (dest[bytes_read] = buffer->data[bytes_read]) != EOF; bytes_read++);

    buffer->current_index -= bytes_read;
    
    return bytes_read;
}



void buffer_reset(buffer_t buffer) {

    if(not_null(buffer))    {
        
        buffer->current_index = 0;
    }
}
    


bool buffer_is_standard(buffer_t buffer)   {

    return not_null(buffer) && buffer->is_standard;
}



bool buffer_unref(buffer_t buffer)   {

    if(is_null(buffer) || buffer->processes_referencing_it == 0) {

        return false;
    }

    buffer->processes_referencing_it--;

    if(!buffer_is_standard(buffer) && buffer->processes_referencing_it == 0)    {

        buffer_free(buffer);
        return false;
    }

    return true;
}



void buffer_ref(buffer_t buffer)    {

    if(not_null(buffer))    {

        buffer->processes_referencing_it++;
    }
}

static inline bool is_null(buffer_t buffer) {

    return buffer == NULL; 
}

static inline bool not_null(buffer_t buffer)    {

    return !is_null(buffer);
}