#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <error.h>
#include <stdint.h>
#include <inttypes.h>

#include "imu.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    if (argc != 1)
        usage(argv);
    
    MPU6050_setup();

    return EXIT_SUCCESS;
}
