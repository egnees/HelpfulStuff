#include <linux/futex.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

typedef _Atomic uint32_t mutex_t;

void futex_wait(mutex_t *s, uint32_t old) {
    syscall(SYS_futex, s, FUTEX_WAIT_PRIVATE, old, NULL, NULL, NULL);
}

void futex_wake(mutex_t *s) {
    syscall(SYS_futex, s, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, NULL);
}

void mutex_init(mutex_t *m) {
    *m = 0;
}

void mutex_lock(mutex_t *m) {
    while (atomic_exchange(m, 1) != 0) {
        futex_wait(m, 1);
    }
}

void mutex_unlock(mutex_t *m) {
    atomic_store(m, 0);
    futex_wake(m);
}
