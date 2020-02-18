#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>

int hello(void *arg) {
    (void) arg;
    puts("Hello world!");
    return 0;
}

int main(void) {
    uint64_t stack_size = 0x1000;
    void *stack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED)
        return 1;

    void *stack_top = (char *) stack + stack_size;
    int tid = clone(hello, stack_top, SIGCHLD, NULL);
    if (tid == -1)
        return 1;

    return 0;
}
