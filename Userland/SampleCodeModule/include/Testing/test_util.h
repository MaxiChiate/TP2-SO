//test_util.h

#ifndef __TEST_UTIL_H
#define __TEST_UTIL_H

    #include <stdint.h>
    #include <processlib.h>

    uint32_t GetUint();
    uint32_t GetUniform(uint32_t max);
    uint8_t memcheck(void *start, uint8_t value, uint32_t size);
    int64_t satoi(char *str);
    void bussy_wait(uint64_t n);
    void endless_loop();
    void endless_loop_print();
    void iotas(int n, char *str, int dim);

#endif