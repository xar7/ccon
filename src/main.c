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
#include "include/cgroup.h"
#include "include/container.h"

int main(int argc, char **argv) {

    struct container_option options = {
        .hostname = "ccontainer",
        .rootfs = "cconfs",
        .stack_size = 0x1000,
    };

    LOG("Creating the (sub)cgroun ccon.");
    if (set_cgroup(&options) != 0) {
        return 1;
    }

    LOG("Initializing stack_size to: 0x%lx.", options.stack_size);
    void *stack = mmap(NULL, options.stack_size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);

    if (stack == MAP_FAILED)
        return 1;

    struct container_arg container_arg = {
        .argc = argc - 1,
        .argv = argv + 1,
        .opt = &options,
    };
    void *stack_top = (char *) stack + options.stack_size;
    pid_t tid = clone(container_function, stack_top,
                      CLONE_NEWUTS | CLONE_NEWPID| CLONE_NEWNS | SIGCHLD,
                      &container_arg);
    if (tid == -1) {
        LOGERR("clone failed");
        return 1;
    }

    int wstatus;
    if (waitpid(tid, &wstatus, 0) == -1) {
        LOGERR("waitpid failed!");
    }
    LOG("child process exited with status code: %d", WEXITSTATUS(wstatus));

    return 0;
}
