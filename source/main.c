#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <error.h>
#include <stdint.h>
#include <inttypes.h>

#include "imu.h"
#include "debug.h"
#include "linalg.h"
#include "control.h"

int main(int argc, char *argv[])
{
    if (argc != 1)
        usage(argv);
    
    int m_fd = M_setup();
    int ag_fd = AG_setup();

    //int fd = MPU_setup();
    //read_accels(fd);

    start_time();
    control_init();
    ekf();
    control_destroy();
    end_time();

    return EXIT_SUCCESS;
}
