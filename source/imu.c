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

struct imu_struct imu_s;
char values[128] = {0};

float x_acc_med = 0;
float y_acc_med = 0;
float z_acc_med = 0;
float x_gyro_med = 0;
float y_gyro_med = 0;
float z_gyro_med = 0;

void MPU6050_setup(void)
{
    int fd = i2c_setup(MPU6050_ADDRESS);
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

void read_from_imu(struct imu_struct *imu_s)
{
    int8_t block_data[BLOCK_SIZE] = {0};
    uint8_t new_data;

    new_data = read_byte(WHO_AM_I, &new_data, imu_s->fd);

    if (new_data)
    {
        /* accelerometer */
            read_block(BLOCK_SIZE, OUT_X_L_A, block_data, imu_s->ag_fd);
            imu_s->x_acc = (block_data[0] + (block_data[1] << 8))*imu_s->acc_sens - x_acc_med*setup_flag;
            imu_s->y_acc = (block_data[2] + (block_data[3] << 8))*imu_s->acc_sens - y_acc_med*setup_flag;
            imu_s->z_acc = (block_data[4] + (block_data[5] << 8))*imu_s->acc_sens - z_acc_med*setup_flag;

        /* gyroscope */
            read_block(BLOCK_SIZE, OUT_X_L_G, block_data, imu_s->ag_fd);

            imu_s->x_gyro = (block_data[0] + (block_data[1] << 8))*imu_s->gyro_sens - x_gyro_med*setup_flag;
            imu_s->y_gyro = (block_data[2] + (block_data[3] << 8))*imu_s->gyro_sens - y_gyro_med*setup_flag;
            imu_s->z_gyro = (block_data[4] + (block_data[5] << 8))*imu_s->gyro_sens - z_gyro_med*setup_flag;
    }
}

void imu_calibration(void)
{
    INFO("IMU calibration...");

    uint16_t calib_counter = 0;
    float x_acc_calib[IMU_CALIB_SIZE] = {0};
    float y_acc_calib[IMU_CALIB_SIZE] = {0};
    float z_acc_calib[IMU_CALIB_SIZE] = {0};
    float x_gyro_calib[IMU_CALIB_SIZE] = {0};
    float y_gyro_calib[IMU_CALIB_SIZE] = {0};
    float z_gyro_calib[IMU_CALIB_SIZE] = {0};

    while (calib_counter < IMU_CALIB_SIZE)
    {
        usleep(IMU_CALIB_US_SLEEP);
        read_from_imu(&imu_s);

        x_acc_calib[calib_counter] = get_x_acc(FALSE);
        y_acc_calib[calib_counter] = get_y_acc(FALSE);
        z_acc_calib[calib_counter] = get_z_acc(FALSE);
        x_gyro_calib[calib_counter] = get_x_gyro(FALSE);
        y_gyro_calib[calib_counter] = get_y_gyro(FALSE);
        z_gyro_calib[calib_counter] = get_z_gyro(FALSE);

        calib_counter++;
    }
    
    x_acc_med = compute_median(x_acc_calib, IMU_CALIB_SIZE);
    y_acc_med = compute_median(y_acc_calib, IMU_CALIB_SIZE);
    z_acc_med = compute_median(z_acc_calib, IMU_CALIB_SIZE);
    x_gyro_med = compute_median(x_gyro_calib, IMU_CALIB_SIZE);
    y_gyro_med = compute_median(y_gyro_calib, IMU_CALIB_SIZE);
    z_gyro_med = compute_median(z_gyro_calib, IMU_CALIB_SIZE);

    pitch0 = asin(-x_acc_med);
    roll0 = asin(y_acc_med/cos(pitch0));

    INFO("x_acc_med: %.4f", x_acc_med);
    INFO("y_acc_med: %.4f", y_acc_med);
    INFO("z_acc_med: %.4f", z_acc_med);
    INFO("x_gyro_med: %.4f", x_gyro_med);
    INFO("y_gyro_med: %.4f", y_gyro_med);
    INFO("z_gyro_med: %.4f", z_gyro_med);
    INFO("pitch0: %.4f", pitch0);
    INFO("roll0: %.4f\n", roll0);
}

void *imu_f(void *p)
{
	INFO("imu_f");
    p = p;  // avoid 'unused argument' warning

    int ag_fd, m_fd;
    
    // setup I2C devices
    ag_fd = i2c_setup(AG_I2C_ADDR);

    // setup gyroscope, accelerometer and magnetometer
    // DO NOT CHANGE THIS ORDER, otherwise expect UNDEFINED BEHAVIOUR
    gyro_init(ag_fd);
    accel_init(ag_fd);

    imu_s.ag_fd = ag_fd;
    imu_s.acc_sens = ACCEL_2G;
    imu_s.gyro_sens = GYRO_245DPS;

    imu_calibration();
    start_time();

    for (;;)
    {
        read_from_imu(&imu_s);
        sprintf(values, "%.7f %.6f %.6f\t%.6f %.6f %.6f\t%.6f %.6f %.6f\t%ld\n", get_x_acc(TRUE), get_y_acc(TRUE), get_z_acc(TRUE), get_x_vel(TRUE), get_y_vel(TRUE), get_z_vel(TRUE), get_x_gyro(TRUE), get_y_gyro(TRUE), get_z_gyro(TRUE), elapsed); 
//        DATA("\r%s", values);

        start_time();
        end_time(FALSE);

        while (elapsed < IMU_SLEEP_US)
            end_time(FALSE);
    }

    pthread_exit(NULL);
}

/* ACC */
float get_x_acc(bool dead_flag)
{
    if (still_setting_up)
        return imu_s.x_acc;

    return (fabs(imu_s.x_acc) < acc_deadzone*dead_flag ? 0.0 : imu_s.x_acc);
}

float get_y_acc(bool dead_flag)
{
    if (still_setting_up)
        return imu_s.y_acc;

    return (fabs(imu_s.y_acc) < acc_deadzone*dead_flag ? 0.0 : imu_s.y_acc);
}

float get_z_acc(bool dead_flag)
{
    if (still_setting_up)
        return imu_s.z_acc;

    return (fabs(imu_s.z_acc) < acc_deadzone*dead_flag ? 0.0 : imu_s.z_acc);
}

/* GYRO */
float get_x_gyro(bool dead_flag)
{
    if (still_setting_up)
        return imu_s.x_gyro;

    return (fabs(imu_s.x_gyro) < gyro_deadzone*dead_flag ? 0.0 : imu_s.x_gyro);
}

float get_y_gyro(bool dead_flag)
{
    if (still_setting_up)
        return imu_s.y_gyro;

    return (fabs(imu_s.y_gyro) < gyro_deadzone*dead_flag ? 0.0 : imu_s.y_gyro);
}

float get_z_gyro(bool dead_flag)
{
    if (still_setting_up)
        return imu_s.z_gyro;

    return (fabs(imu_s.z_gyro) < gyro_deadzone*dead_flag ? 0.0 : imu_s.z_gyro);
}
