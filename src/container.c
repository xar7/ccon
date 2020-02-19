#define _GNU_SOURCE
#include <unistd.h>
#include <string.h>

#include "include/container.h"
#include "include/debug.h"

int container_function(void *raw_arg) {
    struct container_arg *arg = (struct container_arg *) raw_arg;
    LOG("Entering child function.");
    const char *hostname = "ccontainer";
    LOG("Setting the container hostname to : %s.", hostname);
    if (sethostname(hostname, strlen(hostname)) < 0) {
        LOGERR("sethostname failed");
        return 1;
    }

    char **exec_argv = arg->argv;
    LOG("Calling execvp for : %s", exec_argv[0]);
    if (execvp(exec_argv[0], exec_argv) == -1) {
        LOGERR("execvp failed");
        return 1;
    }

    return 0;
}
