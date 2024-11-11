#include <stdint.h>
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



#include "communication.h"
#include "registers.h"
#include "mpu9250.h"

static const char *TAG = "MPU_9250";

// Device Methods
/*Takes an empty mpu9250_t struct and inits its functions and parameters*/
esp_err_t Mpu9250( mpu9250_t* mpu,
                        int i2c_address_param,
                        int i2c_scl_param,
                        int i2c_sda_param,
                        int acc_range,
                        int gyro_range,
                        int mag_mode,
                        int mag_res,
                        float mag_declination) {

    // Function pointers assignment

    mpu->measure = mpu9250_measure;

    mpu->Acc.get_sensitivity = acc_get_sensitivity;
    mpu->Acc.set_range = acc_set_range;

    mpu->Gyro.get_sensitivity = gyro_get_sensitivity;
    mpu->Gyro.set_range = gyro_set_range;

    mpu->Mag.set_mode_res = mag_set_mode_resolution;
    mpu->Mag.enable_bypass = mag_enable_bypass;

    esp_err_t i2c_ret = i2c_init(i2c_sda_param, i2c_scl_param);

    if (i2c_ret == ESP_OK){
        mpu->i2c.address = i2c_address_param;
        mpu->i2c.scl = i2c_scl_param;
        mpu->i2c.sda = i2c_sda_param;
    } else {
        ESP_LOGI(TAG, "INIT->I2C ERROR: %d", i2c_ret);
    }

    mpu->Acc.i2c = mpu->i2c;
    mpu->Gyro.i2c = mpu->i2c;
    mpu->Mag.i2c = mpu->i2c;
    mpu->Temp.i2c = mpu->i2c;

    if(mpu->Acc.set_range( &(mpu->Acc), acc_range) != ESP_OK)
        return ESP_FAIL;
    if(mpu->Gyro.set_range(&(mpu->Gyro), gyro_range) != ESP_OK)
        return ESP_FAIL;
    mpu->Mag.enable_bypass(&(mpu->Mag));
    mpu->Mag.set_mode_res(mag_mode, mag_res);


    mpu->Mag.declination = mag_declination;

    printf( "mpu9250 initialized.\n" );

    return ESP_OK;
}

/*@brief: Reads each sensor and updates its values*/
esp_err_t mpu9250_measure(mpu9250_t* mpu) {

    uint8_t sensor_data[14];    // Buffer for accelerometer and gyroscope
    uint8_t mag_data[7];        // Buffer for magnetometer
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    int16_t mag_x, mag_y, mag_z;

    // Read accelerometer, gyroscope, and magnetometer data
    mpu9250_read_bytes(mpu->i2c.address, MPU9250_REG_ACCEL_XOUT_H, sensor_data, 14);
    mpu9250_read_bytes(AK8963_SENSOR_ADDR, AK8963_REG_HXL, mag_data, 7);

    // Parse and calibrate data from sensors
    accel_x = ((sensor_data[0] << 8) | sensor_data[1]) - mpu->Acc.offset.x;
    accel_y = ((sensor_data[2] << 8) | sensor_data[3]) - mpu->Acc.offset.y;
    accel_z = ((sensor_data[4] << 8) | sensor_data[5]) - mpu->Acc.offset.z;

    gyro_x = ((sensor_data[8] << 8) | sensor_data[9]) - mpu->Gyro.offset.x;
    gyro_y = ((sensor_data[10] << 8) | sensor_data[11]) - mpu->Gyro.offset.y;
    gyro_z = ((sensor_data[12] << 8) | sensor_data[13]) - mpu->Gyro.offset.z;

    mag_x = (((int16_t)((uint16_t)mag_data[1] << 8 | (uint16_t)mag_data[0]))) - mpu->Mag.offset.x;
    mag_y = (((int16_t)((uint16_t)mag_data[3] << 8 | (uint16_t)mag_data[2]))) - mpu->Mag.offset.y;
    mag_z = (((int16_t)((uint16_t)mag_data[5] << 8 | (uint16_t)mag_data[4]))) - mpu->Mag.offset.z;


    // Convert raw data to logical values with units
    float acc_sensitivity = (mpu->Acc.get_sensitivity) (mpu->Acc);
    float gyro_sensitivity = (mpu->Gyro.get_sensitivity) (mpu->Gyro);
    float mag_sensitivity = (mpu->Mag.resolution);

    float accel_x_g = accel_x * acc_sensitivity;
    float accel_y_g = accel_y * acc_sensitivity;
    float accel_z_g = accel_z * acc_sensitivity;

    float gyro_x_dps = gyro_x * gyro_sensitivity;
    float gyro_y_dps = gyro_y * gyro_sensitivity;
    float gyro_z_dps = gyro_z * gyro_sensitivity;

    float mag_x_ut = mag_x * mag_sensitivity;
    float mag_y_ut = mag_y * mag_sensitivity;
    float mag_z_ut = mag_z * mag_sensitivity;

    // Upate sensor values

    // Accelerometer
    mpu->Acc.x = accel_x_g;
    mpu->Acc.y = accel_y_g;
    mpu->Acc.z = accel_z_g;

    // Gyroscopoe
    mpu->Gyro.x = gyro_x_dps;
    mpu->Gyro.y = gyro_y_dps;
    mpu->Gyro.z = gyro_z_dps;

    // Magnetometer
    mpu->Mag.x = mag_x_ut;
    mpu->Mag.y = mag_y_ut;
    mpu->Mag.z = mag_z_ut;

    return ESP_OK;
}


// Sensor Methods
esp_err_t acc_set_range(acc_t* acc, uint8_t fs_sel) {
    if (fs_sel > 3) {
        return ESP_ERR_INVALID_ARG;
    }
    esp_err_t ret = mpu9250_write_byte(acc->i2c.address, ACCEL_CONFIG, fs_sel << 3);
    if (ret != ESP_OK) {
        ESP_LOGE( TAG, "Failed to set_accel_range: %s\n", esp_err_to_name(ret));
    }
    return ret;
}

float acc_get_sensitivity(acc_t acc) {
    uint8_t accel_sensitivity_setting;
    mpu9250_read_bytes(acc.i2c.address, ACCEL_CONFIG, &accel_sensitivity_setting, 1);

    switch (accel_sensitivity_setting & 0x18) {
        case 0x00: return 2.0 / 32768.0;
        case 0x08: return 4.0 / 32768.0;
        case 0x10: return 8.0 / 32768.0;
        case 0x18: return 16.0 / 32768.0;
        default:   return 2.0 / 32768.0;  // Default sensitivity
    }


};

esp_err_t gyro_set_range(gyro_t* gyro, uint8_t fs_sel){
    if (fs_sel > 3) {
        return ESP_ERR_INVALID_ARG;
    }
    esp_err_t ret = mpu9250_write_byte(gyro->i2c.address, GYRO_CONFIG, fs_sel << 3);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set_gyro_range: %s\n", esp_err_to_name(ret));
    }
    return ret;

}
float gyro_get_sensitivity(gyro_t gyro){
    uint8_t gyro_sensitivity_setting;
    mpu9250_read_bytes(gyro.i2c.address, GYRO_CONFIG, &gyro_sensitivity_setting, 1);

    switch (gyro_sensitivity_setting & 0x18) {
        case 0x00:  // ±250 °/s
            return 250.0 / 32768.0;
        case 0x08:  // ±500 °/s
            return 500.0 / 32768.0;
        case 0x10:  // ±1000 °/s
            return 1000.0 / 32768.0;
        case 0x18:  // ±2000 °/s
            return 2000.0 / 32768.0;
        default:
            return 250.0 / 32768.0;  // Default sensitivity
    }

}

esp_err_t mag_enable_bypass(mag_t* mag) {
    // activate bypass mode by writing `0x02` to `INT_PIN_CFG` register (`0x37`) of the MPU-9250 
    return mpu9250_write_byte(mag->i2c.address, 0x37, 0x02);

}
esp_err_t mag_set_mode_resolution(uint8_t mode, uint8_t resolution) {
    // Combine mode and resolution into one byte
    uint8_t mode_and_resolution = mode | resolution;

    // Reset AK8963
    esp_err_t ret = mpu9250_write_byte(AK8963_SENSOR_ADDR, AK8963_REG_CNTL1, AK8963_POWER_DOWN); // Reset device
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to reset AK8963: %s", esp_err_to_name(ret));
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay to allow reset to complete

    // Set mode and resolution
    ret = mpu9250_write_byte(AK8963_SENSOR_ADDR, AK8963_REG_CNTL1, mode_and_resolution);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set mode and resolution: %s", esp_err_to_name(ret));
        return ret;
    }
    
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay to allow reset to complete
    return ESP_OK;
}

float get_mag_sensitivity(){
    uint8_t resolution_setting;
    mpu9250_read_bytes(AK8963_SENSOR_ADDR, AK8963_REG_CNTL1, &resolution_setting, 1);

    // Determine if using 16-bit output (0x10) or defaulting to 14-bit
    if ((resolution_setting & AK8963_BIT_16) == AK8963_BIT_16) {
        return 0.15; // Sensitivity for 16-bit output in µT/LSB
    } else {
        return 0.6; // Sensitivity for 14-bit output in µT/LSB (default)
    }

}

