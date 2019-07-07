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

t_imu_struct imu_s;
bool setup_flag = FALSE;
char values[128] = {0};

float x_acc_med = 0;
float y_acc_med = 0;
float z_acc_med = 0;
float x_gyro_med = 0;
float y_gyro_med = 0;
float z_gyro_med = 0;

float acc_deadzone = -1;
float gyro_deadzone = -1;
float pitch0 = 0;
float roll0 = 0;

void MPU6050_setup(void)
{
    imu_s.fd = i2c_setup(MPU6050_ADDRESS);
}

int i2c_setup(int device_address)
{
    int fd = open_file(I2C_BUS);

    if (ioctl(fd, I2C_SLAVE, device_address) == -1)
    {
        char err_msg[64];
        sprintf(err_msg, "i2c_setup | ioctl [0x%X]", device_address);
        error_handler(err_msg);
    }

    return fd;
}

void read_block(ssize_t block_size, uint8_t reg, int8_t *dest, int fd)
{
    ssize_t left = block_size;
    int r = 0;

    if (write(fd, (char *)&reg, 1) == -1)
        error_handler("read_block | write");
    
    while (left)
    {
        r = read(fd, dest, left);

        if (r == -1)
            error_handler("read_block | read");

        dest += r;
        left -= r;
    }
}

void read_byte(uint8_t reg, int8_t *dest, int fd)
{
    *dest = i2c_smbus_read_byte_data(fd, reg);
}

void read_word(uint8_t reg, int16_t *dest, int fd)
{
    *dest = i2c_smbus_read_word_data(fd, reg);
}

void read_data_block(uint8_t reg, uint8_t *dest, int fd)
{
    i2c_smbus_read_block_data(fd, reg, dest);
}

void write_byte(uint8_t reg, int8_t data, int fd)
{
    i2c_smbus_write_word_data(fd, reg, data);
}

void write_word(uint8_t reg, int16_t data, int fd)
{
    i2c_smbus_write_word_data(fd, reg, data);
}

void check_whoami(int fd)
{
    int8_t who_am_i;

    read_byte(WHO_AM_I, &who_am_i, fd);

    if (who_am_i != 0x68)
        ERROR("who_am_i [0x%2X] != 0x68", who_am_i);
    else
        INFO("who_am_i: 0x%2X", who_am_i);
}

void read_from_imu(t_imu_struct *imu_s)
{
    int8_t block_data[BLOCK_SIZE] = {0};
    int8_t new_data;

    read_byte(INT_STATUS, &new_data, imu_s->fd);

    if (new_data)
    {
        /* accelerometer */
        read_block(BLOCK_SIZE, ACCEL_XOUT_H, block_data, imu_s->fd);
        imu_s->x_acc = (block_data[0] + (block_data[1] << 8))*imu_s->acc_sens - x_acc_med*setup_flag;
        imu_s->y_acc = (block_data[2] + (block_data[3] << 8))*imu_s->acc_sens - y_acc_med*setup_flag;
        imu_s->z_acc = (block_data[4] + (block_data[5] << 8))*imu_s->acc_sens - z_acc_med*setup_flag;

        /* gyroscope */
        read_block(BLOCK_SIZE, GYRO_XOUT_H, block_data, imu_s->fd);

        imu_s->x_gyro = (block_data[1] + (block_data[0] << 8))*imu_s->gyro_sens - x_gyro_med*setup_flag;
        imu_s->y_gyro = (block_data[3] + (block_data[2] << 8))*imu_s->gyro_sens - y_gyro_med*setup_flag;
        imu_s->z_gyro = (block_data[5] + (block_data[4] << 8))*imu_s->gyro_sens - z_gyro_med*setup_flag;
    }
}

/* ACC */
float get_x_acc(void)
{
    return imu_s.x_acc;
}

float get_y_acc(void)
{
    return imu_s.y_acc;
}

float get_z_acc(void)
{
    return imu_s.z_acc;
}

/* GYRO */
float get_x_gyro(void)
{
    return imu_s.x_gyro;
}

float get_y_gyro(void)
{
    return imu_s.y_gyro;
}

float get_z_gyro(void)
{
    return imu_s.z_gyro;
}
