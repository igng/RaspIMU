#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>

#include "imu.h"
#include "smbus.h"
#include "debug.h"
#include "utils.h"

int open_file(const char *filename)
{
    int flags = O_CREAT | O_RDWR | O_TRUNC;
    int permissions = 00666;
    int fd = open(filename, flags, permissions); 
    char err_msg[64];

    if (fd == -1)
    {    
        sprintf(err_msg, "open_file | open [%s]", filename);
        error_handler(err_msg);
    }    

    return fd;
}

float compute_median(float *v, int len) 
{
    float temp_v[len];

    memcpy(temp_v, v, sizeof(float)*len);
    sort_array(temp_v, len);

    if (len%2)
        return temp_v[(len+1)/2 - 1];
    else 
        return (temp_v[(len+1)/2] + temp_v[(len+1)/2 - 1])/2;
}

void swap(float *a, float *b)
{
    float temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void sort_array(float *v, int len) 
{
    int i, j;

    for(i = 0; i < len - 1; i++) 
        for(j = 0; j < len-i-1; j++) 
            if(v[j] > v[j+1])
                swap(v+j, v+j+1);
}
