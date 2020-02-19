#include <stdio.h>


/* Not so clean debugging MACRO */
#ifdef DEBUG
#define LOG(...) fprintf(stderr, "[DEBUG] ");     \
    fprintf(stderr, __VA_ARGS__);                 \
    putchar('\n');

#else
#define LOG(...)
#endif /* DEBUG */
