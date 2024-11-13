// buffer.c
#include <Collections/buffer.h>

#define MUTEX_INITIAL 	1                              
#define SYNC_INITIAL 	0

#define AVAILABLE_SPACE(s, x, y) (((x) >= (y)) ? ((s) - (x) + (y)) : ((y) - (x)))
#define AVAILABLE_DATA(s, x, y) (((x) >= (y)) ? ((x) - (y)) : ((s) - (y) + (x)))


struct buffer {

    char * data;

    int write_index;
    int read_index;

    uint32_t size;

    int8_t mutex;	
	int8_t sync; 

//Cuantos procesos lo usan:
    int processes_referencing_it;
};

static inline bool buffer_is_null(buffer_t buffer);
static inline bool buffer_not_null(buffer_t buffer);

static int read_memcpy(char * dest, const char * src, unsigned int size);


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

    buffer->write_index = 0;
    buffer->read_index = 0;
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
    
        if (size > AVAILABLE_SPACE(buffer->size, buffer->write_index, buffer->read_index)) {

            up(buffer->mutex);
            return -1;
        }

    // Si quiere escribir mas del espacio que queda, escribe hasta el final 
    // y escribe el resto desde el princpio de data. Actuando como buffer circular:

        if(size > buffer->size - buffer->write_index    )   {
    
            int left_space = buffer->size - buffer->write_index;

            memcpy(buffer->data + buffer->write_index, src, left_space);    

            size -= left_space;
            src += left_space;
            buffer->write_index = 0;
        }

        memcpy(buffer->data + buffer->write_index, src, size);
        
        buffer->write_index += size;

    up(buffer->mutex);

    upNtimes(buffer->sync, size);

    return size;  
}



int buffer_read(buffer_t buffer, char *dest, int size)  {

    if (buffer_is_null(buffer) || dest == NULL) {

        return -1;
    }

    downNtimes(buffer->sync, size);

    down(buffer->mutex);

    // Si el tamaño a leer excede el espacio hasta el final del buffer, realiza lectura circular:

        if (size > buffer->size - buffer->read_index)   {
            
            int left_space = buffer->size - buffer->read_index;

            read_memcpy(dest, buffer->data + buffer->read_index, left_space);    

            size -= left_space;
            dest += left_space;
            buffer->read_index = 0;
        }

        read_memcpy(dest, buffer->data + buffer->read_index, size);
        buffer->read_index += size;
        
    up(buffer->mutex);

    return size;
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


int buffer_read_all(buffer_t buffer, char *dest)    {

    if (buffer_is_null(buffer) || dest == NULL) {

        return -1;  
    }

    int available_data = AVAILABLE_DATA(buffer->size, buffer->write_index, buffer->read_index);

    return buffer_read(buffer, dest, available_data);
}





static inline bool buffer_is_null(buffer_t buffer) {

    return buffer == NULL; 
}

static inline bool buffer_not_null(buffer_t buffer)    {

    return !buffer_is_null(buffer);
}

static int read_memcpy(char * dest, const char * src, unsigned int size)   {

    int bytes_cpy;

// Copia todo el size y frena si hay un EOF (tambien lo copia):
    for (bytes_cpy = 0; bytes_cpy < size && (dest[bytes_cpy] = src[bytes_cpy]) != EOF; bytes_cpy++);

    return bytes_cpy;
}