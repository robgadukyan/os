#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void check_thread_call(int result, const char *operation)
{
    if (result != 0) {
        fprintf(stderr, "%s failed: %s\n", operation, strerror(result));
        exit(EXIT_FAILURE);
    }
}

static void join_threads(pthread_t threads[], size_t count)
{
    for (size_t i = 0; i < count; ++i) {
        check_thread_call(pthread_join(threads[i], NULL), "pthread_join");
    }
}

#endif
