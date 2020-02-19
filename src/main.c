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

int child_function(void *arg) {
    (void) arg;
    LOG("Entering child function.");
    char *hostname = "ccontainer";
    LOG("Setting the container hostname to : %s.", hostname);
    if (sethostname(hostname, strlen(hostname)) < 0) {
        LOGERR("sethostname failed");
        return 1;
    }

    char **exec_argv = (char **) arg;
    LOG("Calling execvp for : %s", exec_argv[0]);
    if (execvp(exec_argv[0], exec_argv) == -1) {
        LOGERR("execvp failed");
        return 1;
    }

    return 0;
}

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
