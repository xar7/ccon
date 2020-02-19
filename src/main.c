#define _GNU_SOURCE
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "include/debug.h"
#include "include/container.h"

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    uint64_t stack_size = 0x10000;

    LOG("Initializing stack_size to: 0x%lx.", stack_size);
    void *stack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);

    if (stack == MAP_FAILED)
        return 1;

    void *stack_top = (char *) stack + stack_size;
    pid_t tid = clone(child_function, stack_top, CLONE_NEWUTS | CLONE_NEWUSER | SIGCHLD, argv + 1);
    if (tid == -1) {
        LOGERR("Clone failed");
        return 1;
    }

    int wstatus;
    if (waitpid(tid, &wstatus, 0) == -1) {
        LOGERR("Waitpid failed!");
    }

    return 0;
}
