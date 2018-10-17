#include <stdlib.h>

#include "imu.h"
#include "debug.h"
#include "timer.h"
#include "control.h"

int main(int argc, char *argv[])
{
    if (argc != 1)
        usage(argv);
    
//    int fd = MPU_setup();
//    read_accels(fd);

    timer_init(PERIOD);
    control_init();

    while (TRUE)
        pause();

    control_destroy();
    return EXIT_SUCCESS;
}
