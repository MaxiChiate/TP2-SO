// buffer.h

#ifndef BUFFER_H
#define BUFFER_H

    #include <memory_manager.h>
    #include <lib.h>
    #include <stdint.h>

    typedef struct buffer * buffer_t;

    buffer_t buffer_init(uint32_t size, bool is_standard);
    void buffer_free(buffer_t buffer);

//El leer consume.
    int buffer_write(buffer_t buffer, const char *src, int size);
    int buffer_read(buffer_t buffer, char *dest, int size);

    void buffer_reset(buffer_t buffer);

// Asume que se está referenciado por alguien, retorna false si ya nadie mas lo referencia
// Hace buffer_free si lo referencia nadie.
    bool buffer_unref(buffer_t buffer);
    void buffer_ref(buffer_t buffer);

    bool buffer_is_standard(buffer_t buffer);

#endif // BUFFER_H
