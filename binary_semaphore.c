#include "binary_semaphore.h"
#include <semaphore.h>
#include <atomic.h>

struct bin_sem {
    sem_t sem;
};

void bin_sem_post(sem_t * s) {
    sem_trywait(s);
    sem_post(s);
}

void bin_sem_wait(bin_sem * s) {
    acquire_wait_lock
    sem_wait
    for i in sem_getvalue
    sem_trywait
    release_wait_lock
}

bin_sem * bin_sem_new(bool available) {
    bin_sem * s = malloc(sizeof(bin_sem));

    ATOMIC_FLAG_INIT(&s->status);
    if (available) {
        atomic_flag_test_and_set_explicit(&s->status, memory_order_relaxed);
    } else {
        atomic_flag_clear_explicit(&s->status, memory_order_relaxed);
    }
    sem_init(s->sem, 0, (available) ? 1 : 0);
    return s;
}

void bin_sem_free(bin_sem * s) {
    sem_destroy(&s->sem);
    free(s);
}

void bin_sem_wait(bin_sem * s) {
    sem_wait(&s->sem);
    atomic_flag_clear_explicit(&s->status, memory_order_relaxed);
}

void bin_sem_post(bin_sem * s) {
    if (atomic_flag_test_and_set_explicit(&s->status, memory_order_relaxed)) {
        sem_post(&s->sem);
    }
}
