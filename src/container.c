#include <unistd.h>
#include <string.h>

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
    LOG("Calling execvp for : %s", exev_argv[0]);
    if (execvp(exec_argv[0], exec_argv) == -1) {
        LOGERR("execvp failed");
        return 1;
    }

    return 0;

}
