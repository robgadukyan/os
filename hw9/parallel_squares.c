#include "threads.h"

static void *print_square(void *argument)
{
    const int number = *(const int *)argument;
    printf("Square of %d is %d\n", number, number * number);
    return NULL;
}

int main(void)
{
    int numbers[] = {1, 2, 3, 4, 5};
    pthread_t threads[sizeof numbers / sizeof numbers[0]];
    const size_t count = sizeof numbers / sizeof numbers[0];

    for (size_t i = 0; i < count; ++i) {
        check_thread_call(pthread_create(&threads[i], NULL, print_square, &numbers[i]),
                          "pthread_create");
    }
    join_threads(threads, count);
    return EXIT_SUCCESS;
}
