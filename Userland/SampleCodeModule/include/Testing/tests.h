#ifndef __TESTING_H
#define __TESTING_H

    #include <Testing/test_util.h>
    #include <semlib.h>
    #include <processlib.h>
    #include <heaplib.h>

    #define END_MESSAGE "OK\n"

    void test_processes(int argc, char ** argv);
    void test_prio(int argc, char ** argv);
    void test_sync(int argc, char ** argv);
    void test_async(int argc, char ** argv);
    void test_mm(int argc, char ** argv);

#endif