// buffer.c
#include <Collections/buffer.h>

#define MUTEX_INITIAL 	1                              
#define SYNC_INITIAL 	0

struct buffer {

    char * data;
    int current_index;
    uint32_t size;

    int8_t mutex;	
	int8_t sync; 

//Cuantos procesos lo usan:
    int processes_referencing_it;
};

static inline bool buffer_is_null(buffer_t buffer);
static inline bool buffer_not_null(buffer_t buffer);


buffer_t buffer_init(uint32_t size) {

    if(size == 0)   {

        return NULL;
    }

    buffer_t buffer = (buffer_t) mm_malloc(sizeof(*buffer));

    if (!buffer) {
    
        return NULL;
    }

    buffer->data = (char *) mm_malloc(size*sizeof(buffer->data[0]));
    buffer->mutex = new_sem(MUTEX_INITIAL);
    buffer->sync  = new_sem(SYNC_INITIAL);

    if (!buffer->data || buffer->mutex < 0 || buffer->sync < 0) {
    
        kill_sem(buffer->mutex);
        kill_sem(buffer->sync);
        mm_free(buffer->data); // Si es null no pasa nada
        mm_free(buffer);
        return NULL;
    }

    buffer->current_index = 0;
    buffer->size = size;
    buffer->processes_referencing_it = 1;

    return buffer;
}



void buffer_free(buffer_t buffer) {

    if (buffer_not_null(buffer)) {

        kill_sem(buffer->mutex);
        kill_sem(buffer->sync);
        mm_free(buffer->data);
        mm_free(buffer);
    }
}



int buffer_write(buffer_t buffer, const char *src, int size)   {

    if(buffer_is_null(buffer) || src == NULL)  {
        
        return -1;
    }

    down(buffer->mutex);

        if ((buffer->current_index + size) > buffer->size) {

            size = buffer->size - buffer->current_index;
        }


        memcpy(buffer->data + buffer->current_index, src, size);
        
        buffer->current_index += size;

    up(buffer->mutex);

    upNtimes(buffer->sync, size);

    return size;  
}



int buffer_read(buffer_t buffer, char *dest, int size) {

    if(buffer_is_null(buffer) || dest == NULL) {

        return -1;
    }

    down(buffer->mutex);

        if (size > buffer->current_index)   {

            size = buffer->current_index;
        }

    up(buffer->mutex);

    downNtimes(buffer->sync, size);

    down(buffer->mutex);

        int bytes_read;

    // Copia todo el size y frena si hay un EOF (tambien lo copia):
        for (bytes_read = 0; bytes_read < size && 
            (dest[bytes_read] = buffer->data[bytes_read]) != EOF; bytes_read++);

        buffer->current_index -= bytes_read;
    
    up(buffer->mutex);

    return bytes_read;
}



bool buffer_unref(buffer_t buffer)   {

    if(buffer_is_null(buffer)) {

        return false;
    }

    down(buffer->mutex);

        buffer->processes_referencing_it--;

        if(buffer->processes_referencing_it <= 0)    {

            up(buffer->mutex);
            buffer_free(buffer);
            return false;
        }

    up(buffer->mutex);

    return true;
}



void buffer_ref(buffer_t buffer)    {

    if(buffer_not_null(buffer))    {

        down(buffer->mutex);
            
            buffer->processes_referencing_it++;
        
        up(buffer->mutex);
    }
}

static inline bool buffer_is_null(buffer_t buffer) {

    return buffer == NULL; 
}

static inline bool buffer_not_null(buffer_t buffer)    {

    return !buffer_is_null(buffer);
}