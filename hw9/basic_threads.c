#include "threads.h"

enum { THREAD_COUNT = 3 };

static void *announce(void *argument)
{
    const int worker = *(const int *)argument;
    printf("Thread %d (pthread ID %lu) is running\n",
           worker, (unsigned long)pthread_self());
    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];
    int workers[THREAD_COUNT] = {1, 2, 3};

    for (size_t i = 0; i < THREAD_COUNT; ++i) {
        check_thread_call(pthread_create(&threads[i], NULL, announce, &workers[i]),
                          "pthread_create");
    }
    join_threads(threads, THREAD_COUNT);
    puts("Main: all three threads finished.");
    return EXIT_SUCCESS;
}
