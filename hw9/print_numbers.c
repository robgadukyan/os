#include "threads.h"

enum { THREAD_COUNT = 3 };

static void *print_sequence(void *argument)
{
    const int worker = *(const int *)argument;
    const unsigned long thread_id = (unsigned long)pthread_self();

    for (int number = 1; number <= 5; ++number) {
        printf("Thread %d (pthread ID %lu): %d\n", worker, thread_id, number);
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];
    int workers[THREAD_COUNT] = {1, 2, 3};

    for (size_t i = 0; i < THREAD_COUNT; ++i) {
        check_thread_call(pthread_create(&threads[i], NULL, print_sequence, &workers[i]),
                          "pthread_create");
    }
    join_threads(threads, THREAD_COUNT);
    return EXIT_SUCCESS;
}
