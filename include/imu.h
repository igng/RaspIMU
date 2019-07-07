#ifndef IMU_H_
#define IMU_H_

#include <stdint.h>

#define BLOCK_SIZE 6

#define I2C_BUS "/dev/i2c-1"
#define MPU6050_ADDRESS 0x68

/* MPU6050 registers */
/* Please refer to the detasheet for more info about the registers: <datasheet link> */
#define SMPRT_DIV           0x19
#define CONFIG              0x1A
#define GYRO_CONFIG         0x1B
#define ACCEL_CONFIG        0x1C
#define FIFO_EN_REG         0x23
// [FOR NOW] skipping the I2C master/slave pins
#define INT_PIN_CFG         0x37
#define INT_ENABLE          0x38
#define INT_STATUS          0x3A
#define ACCEL_XOUT_H        0x3B
#define ACCEL_XOUT_L        0x3C
#define ACCEL_YOUT_H        0x3D
#define ACCEL_YOUT_L        0x3E
#define ACCEL_ZOUT_H        0x3F
#define ACCEL_ZOUT_L        0x40
#define TEMP_OUT_H          0x41
#define TEMP_OUT_L          0x42
#define GYRO_XOUT_H         0x43
#define GYRO_XOUT_L         0x44
#define GYRO_YOUT_H         0x45
#define GYRO_YOUT_L         0x46
#define GYRO_ZOUT_H         0x47
#define GYRO_ZOUT_L         0x48
#define SIGNAL_PATH_RESET   0x68
#define USER_CTRL           0x6A
#define PWR_MGMT_1          0x6B
#define PWM_MGMT_2          0x6C
#define FIFO_COUNT_H        0x72
#define FIFO_COUNT_L        0x73
#define FIFO_R_W            0x74
#define WHO_AM_I            0x75

/* MPU6050 config values */
#define FS_SEL_250DPS   0
#define FS_SEL_500DPS   1
#define FS_SEL_1000DPS  2
#define FS_SEL_2000DPS  3
#define AFS_SEL_2G      0      
#define AFS_SEL_4G      1      
#define AFS_SEL_8G      2      
#define AFS_SEL_16      3
#define TEMP_FIFO_EN    1<<7
#define XG_FIFO_EN      1<<6
#define YG_FIFO_EN      1<<5
#define ZG_FIFO_EN      1<<4
#define ACCEL_FIFO_EN   1<<3
#define SLV2_FIFO_EN    1<<2
#define SLV1_FIFO_EN    1<<1
#define SLV0_FIFO_EN    1<<0
#define FIFO_OFLOW_EN   1<<4
#define DATA_RDY_EN     1<<0
#define FIFO_OFLOW_INT  1<<4
#define DATA_RDY_INT    1<<0
#define GYRO_RESET      1<<2
#define ACCEL_RESET     1<<1
#define TEMP_RESET      1<<0
#define FIFO_EN         1<<6
#define FIFO_RESET      1<<2
#define SIG_COND_RESET  1<<0
#define DEVICE_RESET    1<<7
#define SLEEP           1<<6
#define CYCLE           1<<5
#define TEMP_DIS        1<<3
#define STBY_XA         1<<5
#define STBY_YA         1<<4
#define STBY_ZA         1<<3
#define STBY_XG         1<<2
#define STBY_YG         1<<1
#define STBY_ZG         1<<0

#define deg_temp(t) (t/340 + 36.53);

static const uint16_t ACCEL_SENS[4] = {16384, 8192, 4096, 2048};
static const float GYRO_SENS[4] = {131, 65.5, 32.8, 16.4};

typedef struct s_imu_struct
{
    int fd;         // IMU file descriptor
    float acc_sens; // accelerometer sensitivity
    float gyro_sens;// gyroscope sensitivity
    float x_acc;    // acceleration on x axis
    float y_acc;    // acceleration on y ayis
    float z_acc;    // acceleration on z azis
    float x_gyro;   // angular speed on x axis
    float y_gyro;   // angular speed on y ayis
    float z_gyro;   // angular speed on z azis
} t_imu_struct;

void MPU6050_setup(void);
int i2c_setup(int);
void check_whoami(int);

void read_byte(uint8_t, int8_t *, int);
void read_word(uint8_t, int16_t *, int);
void write_byte(uint8_t, int8_t, int);
void write_word(uint8_t, int16_t, int);
void read_block(ssize_t, uint8_t, int8_t *, int);

void accel_init(int);
void gyro_init(int);

float get_x_acc(void);
float get_y_acc(void);
float get_z_acc(void);
float get_x_gyro(void);
float get_y_gyro(void);
float get_z_gyro(void);

#endif
