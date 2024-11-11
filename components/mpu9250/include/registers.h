
#ifndef REGISTERS
#define REGISTERS

// Addresses
#define AK8963_SENSOR_ADDR   0x0C             /*!< Slave address of the AK8963 magnetometer */

// MPU9250 Registers 
#define MPU9250_REG_PWR_MGMT_1   0x6B
#define MPU9250_REG_ACCEL_XOUT_H 0x3B
#define MPU9250_REG_GYRO_XOUT_H  0x43

#define GYRO_RANGE_250_DPS          0
#define GYRO_RANGE_500_DPS          1
#define GYRO_RANGE_1000_DPS         2
#define GYRO_RANGE_2000_DPS         3

#define ACCELEROMETER_RANGE_2G      0      
#define ACCELEROMETER_RANGE_4G      1      
#define ACCELEROMETER_RANGE_8G      2      
#define ACCELEROMETER_RANGE_16G     3

//AK8963 Registers (Magnetometer)
#define AK8963_REG_CNTL1         0x0A
#define AK8963_REG_HXL           0x03
#define AK8963_BIT_14            0x00  /*!< Output 14-bit magnetometer data */
#define AK8963_BIT_16            0x10  /*!< Output 16-bit magnetometer data */
#define AK8963_POWER_DOWN        0x00

//AK8963 Modes
#define AK8963_MODE_POWER_DOWN              0x00    //Power-down mode
#define AK8963_MODE_SINGLE_MEASURE          0x01    //Single measurement mode
#define AK8963_MODE_CONTINUOUS_8_HZ         0x02    //Continuous measurement mode 1 (8 Hz)
#define AK8963_MODE_CONTINUOUS_100_HZ       0x06    //Continuous measurement mode 2 (100 Hz)
#define AK8963_MODE_EXTERNAL_TRIGGER        0x08    //External trigger measurement mode
#define AK8963_MODE_SELF_TEST               0x0F    //Self-test mode
#define AK8963_MODE_FUSE_ROM_ACCESS         0x0B    //Fuse ROM access mode


// Sensor register addresses within the MPU9250 and the AK8963 sensor (which is the magnetometer component of the MPU9250 system).
#define ACCEL_CONFIG 0x1C
#define GYRO_CONFIG 0x1B
#define AK8963_CNTL1 0x0A

#endif