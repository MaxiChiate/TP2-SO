#ifndef __TESTING_H
#define __TESTING_H

    #include <Testing/syscall.h>
    #include <Testing/test_util.h>

    #define MAX_PROCESSES 62

    int64_t test_processes();
    void test_prio();

#endif