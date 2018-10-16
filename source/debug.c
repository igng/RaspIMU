#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

void error_handler(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void usage(char **argv)
{
    fprintf(stderr, "Usage: %s <no arguments>\n", argv[0]);
    exit(EXIT_FAILURE);
}
