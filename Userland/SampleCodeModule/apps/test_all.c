#include <Testing/tests.h>

void test_all(int argc, char ** argv)   {

    int argc1 = 2;
    int argc2 = 1;
    int argc3 = 3;
    int argc4 = 3;
    int argc5 = 3;

    char * argv1[] = {"test_mm", "128", NULL};
    char * argv2[] = {"test_prio", NULL};
    char * argv3[] = {"test_processes", "2", "1", NULL};
    char * argv4[] = {"test_sync", "5", "1", NULL};
    char * argv5[] = {"test_async", "5", "0", NULL};

    int64_t p1 = spawn_process((int64_t) &test_mm, argc1, argv1, 0, 1);
    int64_t p2 = spawn_process((int64_t) &test_prio, argc2, argv2, 0, 1);
    int64_t p3 = spawn_process((int64_t) &test_processes, argc3, argv3, 0, 1);
    int64_t p4 = spawn_process((int64_t) &test_sync, argc4, argv4, 0, 1);
    int64_t p5 = spawn_process((int64_t) &test_sync, argc5, argv5, 0, 1);

    waitpid(p1);
    waitpid(p2);
    waitpid(p3);
    waitpid(p4);
    waitpid(p5);

    suicide();
}