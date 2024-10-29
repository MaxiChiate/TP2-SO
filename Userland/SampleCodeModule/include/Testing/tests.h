#ifndef __TESTING_H
#define __TESTING_H

    #include <Testing/syscall.h>
    #include <Testing/test_util.h>

    #define MAX_PROCESSES 62

    void test_processes(int argc, char ** argv);
    void test_prio(int argc, char ** argv);
    void test_sync(int argc, char ** argv);
    void test_async(int argc, char ** argv);

#endif