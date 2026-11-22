#include "threads.h"

enum { THREAD_COUNT = 2 };

struct sum_task {
    const int *values;
    size_t begin;
    size_t end;
    int worker;
};

static void *sum_slice(void *argument)
{
    const struct sum_task *task = argument;
    int sum = 0;

    for (size_t i = task->begin; i < task->end; ++i) {
        sum += task->values[i];
    }
    printf("Thread %d, indices [%zu, %zu): partial sum = %d\n",
           task->worker, task->begin, task->end, sum);
    return NULL;
}

int main(void)
{
    const int values[] = {1, 2, 3, 4, 5, 6, 7, 8};
    const size_t count = sizeof values / sizeof values[0];
    struct sum_task tasks[THREAD_COUNT] = {
        {values, 0, count / 2, 1},
        {values, count / 2, count, 2}
    };
    pthread_t threads[THREAD_COUNT];

    for (size_t i = 0; i < THREAD_COUNT; ++i) {
        check_thread_call(pthread_create(&threads[i], NULL, sum_slice, &tasks[i]),
                          "pthread_create");
    }
    join_threads(threads, THREAD_COUNT);
    return EXIT_SUCCESS;
}
