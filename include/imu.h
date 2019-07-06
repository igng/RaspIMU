#ifndef IMU_H_
#define IMU_H_

#include <stdint.h>
#include "debug.h"

#define I2C_BUS "/dev/i2c-1"

/* MPU6050 registers */
#define PWR_MGT         0x6B
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40
#define GIRO_XOUT_H     0x43
#define GIRO_XOUT_L     0x44
#define GIRO_YOUT_H     0x45
#define GIRO_YOUT_L     0x46
#define GIRO_ZOUT_H     0x47
#define GIRO_ZOUT_L     0x48

/* xinabos SI01 global registers */
#define AG_I2C_ADDR     0x6A
#define M_I2C_ADDR      0x1C
#define WHO_AM_I_AG     0x0F
#define WHO_AM_I_M      0x0F
/* xinabox SI01 registers [accelerometer + gyroscope] */
#define STATUS_REG_AG   0x17
#define ACC_READY       0x01
#define GYRO_READY      0x02
// gyrosocope
#define CTRL_REG1_G     0x10
#define CTRL_REG2_G     0x11
#define CTRL_REG3_G     0x12
#define ORIENT_CFG_G    0x13
#define CTRL_REG4       0x1E
#define OUT_X_L_G       0x18
#define OUT_X_H_G       0x19
#define OUT_Y_L_G       0x1A
#define OUT_Y_H_G       0x1B
#define OUT_Z_L_G       0x1C
#define OUT_Z_H_G       0x1D
#define GYRO_245DPS     0.00875
#define GYRO_500DPS     0.01750
#define GYRO_2000DPS    0.07000
// acceleromoeter
#define CTRL_REG5_XL    0x1F
#define CTRL_REG6_XL    0x20
#define CTRL_REG7_XL    0x21
#define OUT_X_L_A       0x28
#define OUT_X_H_A       0x29
#define OUT_Y_L_A       0x2A
#define OUT_Y_H_A       0x2B
#define OUT_Z_L_A       0x2C
#define OUT_Z_H_A       0x2D
#define ACCEL_2G        0.000061
#define ACCEL_4G        0.000122
#define ACCEL_8G        0.000244
#define ACCEL_16G       0.000732       // ?? *2, *4, *8, *12
// magnetometer
#define OFFSET_X_REG_L_M    0x05
#define OFFSET_X_REG_H_M    0x06
#define OFFSET_Y_REG_L_M    0x07
#define OFFSET_Y_REG_H_M    0x08
#define OFFSET_Z_REG_L_M    0x09
#define OFFSET_Z_REG_H_M    0x0A
#define WHO_AM_I_M          0x0F
#define CTRL_REG1_M         0x20
#define CTRL_REG2_M         0x21
#define CTRL_REG3_M         0x22
#define CTRL_REG4_M         0x23
#define CTRL_REG5_M         0x24
#define STATUS_REG_M        0x27
#define INT_CFG_M           0x30
#define INT_SRC_M           0x31
#define INT_THS_L_M         0x32
#define INT_THS_H_M         0x33
#define OUT_X_L_M           0x28
#define OUT_X_H_M           0x29
#define OUT_Y_L_M           0x2A
#define OUT_Y_H_M           0x2B
#define OUT_Z_L_M           0x2C
#define OUT_Z_H_M           0x2D

/* xinabox SI01 registers [magnetometer] */
#define MAG_READY           0x08
#define OUT_X_L_M           0x28
#define OUT_X_H_M           0x29
#define OUT_Y_L_M           0x2A
#define OUT_Y_H_M           0x2B
#define OUT_Z_L_M           0x2C
#define OUT_Z_H_M           0x2D
#define MAG_4GAUSS          0.00014
#define MAG_8GAUSS          0.00029
#define MAG_12GAUSS         0.00043
#define MAG_16GAUSS         0.00058

#define IMU_SLEEP_US        1000
#define IMU_CALIB_SIZE      500
#define IMU_CALIB_US_SLEEP  5*MS_TO_US

#define BLOCK_SIZE  6    // 6 registers (x_low, x_high, y_low, y_high, z_low, z_high)
#define ACC_TAU     0.98
#define GYRO_TAU    0.99

typedef struct s_imu_struct
{
    int ag_fd;      // accelerometer+gyroscope file descriptor
    int m_fd;       // magnetometer file descriptor
    float acc_sens; // accelerometer sensitivity
    float gyro_sens;// gyroscope sensitivity
    float mag_sens; // magnetometer sensitivity
    float x_acc;    // acceleration on x axis
    float y_acc;    // acceleration on y ayis
    float z_acc;    // acceleration on z azis
    float x_vel;    // linear velocity on x axis
    float y_vel;    // linear velocity on y ayis
    float z_vel;    // linear velocity on z azis
    float x_gyro;   // angular speed on x axis
    float y_gyro;   // angular speed on y ayis
    float z_gyro;   // angular speed on z azis
    float x_mag;    // magnetic value on x axis
    float y_mag;    // magnetic value on y ayis
    float z_mag;    // magnetic value on z azis
} t_imu_struct;

extern struct t_imu_struct imu;

void setup_z_drift(float, float);

int i2c_setup(int);
void check_whoami(int, int);
void *imu_f(void *);

void read_byte(uint8_t, int8_t *, int);
void read_word(uint8_t, int16_t *, int);
void write_byte(uint8_t, int8_t, int);
void write_word(uint8_t, int16_t, int);
void read_block(uint8_t, int16_t *, int);

void accel_init(int);
void gyro_init(int);
void magnet_init(int);

float get_x_acc(bool);
float get_y_acc(bool);
float get_z_acc(bool);
float get_x_vel(bool);
float get_y_vel(bool);
float get_z_vel(bool);
float get_x_gyro(bool);
float get_y_gyro(bool);
float get_z_gyro(bool);
float get_x_mag(bool);
float get_y_mag(bool);
float get_z_mag(bool);

#endif
