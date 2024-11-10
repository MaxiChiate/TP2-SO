#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <memory_manager.h>

    void * memset(void * destination, int32_t character, uint64_t length);
    void * memcpy(void * destination, const void * source, uint64_t length);
    uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
    void * getcpy(const void * source, uint64_t length, uint16_t data_size);

    // libasm:

    char *cpuVendor(char *result);
    void script();

#endif