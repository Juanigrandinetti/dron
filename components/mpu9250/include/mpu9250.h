#include <stdint.h>
#include "esp_system.h"
#include "registers.h"


// Structures

// Structs

typedef struct offset {
    float x;
    float y;
    float z;
} offset_t;

typedef struct i2c_config {
    int address;
    int scl;
    int sda;

} i2c_config_t;

typedef struct magnetometer {
    float x;
    float y;
    float z;

    i2c_config_t i2c;

    offset_t offset;

    float declination;
    float resolution;
    float mode;

    esp_err_t (*set_mode_res)(uint8_t mag_mode, uint8_t mag_res);
    esp_err_t (*enable_bypass)(struct magnetometer* self);

} mag_t;

typedef struct accelerometer {
    float x;
    float y;
    float z;

    i2c_config_t i2c;

    offset_t offset;

    float range;

    float (*get_sensitivity)(struct accelerometer self);
    esp_err_t (*set_range)(struct accelerometer* self, uint8_t range);

} acc_t;

typedef struct gyroscope {
    float x;
    float y;
    float z;

    i2c_config_t i2c;

    offset_t offset;

    float range;

    float (*get_sensitivity)(struct gyroscope self);
    esp_err_t (*set_range)(struct gyroscope* self, uint8_t range);

} gyro_t;

typedef struct temperature {
    float temperature;
    i2c_config_t i2c;


} temp_t;



typedef struct mpu9250_t {
    // Device config
    i2c_config_t i2c;

    esp_err_t (*measure)(struct mpu9250_t* self);
    
    // Sensors
    acc_t Acc;
    mag_t Mag;
    gyro_t Gyro;
    temp_t Temp;



} mpu9250_t;



//Methods
esp_err_t acc_set_range(acc_t* acc, uint8_t fs_sel);
float acc_get_sensitivity(acc_t acc);

esp_err_t gyro_set_range(gyro_t* gyro, uint8_t fs_sel);
float gyro_get_sensitivity(gyro_t gyro);

esp_err_t mag_enable_bypass(mag_t* mag);
esp_err_t mag_set_mode_resolution(uint8_t mode, uint8_t resolution);
float get_mag_sensitivity();

esp_err_t mpu9250_measure(mpu9250_t* mpu);





esp_err_t Mpu9250( mpu9250_t* mpu,
                        int i2c_address_param,
                        int i2c_scl_param,
                        int i2c_sda_param,
                        int acc_range,
                        int gyro_range,
                        int mag_mode,
                        int mag_res,
                        float mag_declination);