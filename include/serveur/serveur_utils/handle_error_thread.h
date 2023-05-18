#ifndef HANDLE_ERROR_THREAD_H
#define HANDLE_ERROR_THREAD_H

#include <pthread.h>

typedef struct thread_waiter_args {
    pthread_t thread_to_wait; // Le thread à attendre
} thread_waiter_args;

void* thread_waiter(void* args);

#endif //HANDLE_ERROR_THREAD_H
