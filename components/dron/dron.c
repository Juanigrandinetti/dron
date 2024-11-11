#include <stdio.h>
#include <dron.h>
#include <string.h>



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

void dron_calibration( dron_t * self )
{
    printf( "Starting calibration.\n" );
    float avg_roll  = 0;
    float avg_pitch = 0;
    float avg_z     = 0;
    for (int i = 0; i < N_MEASURES; i++)                                            /* Accumulate 'N_MEASURES' measures for Roll, Pitch and z */
    {
        self->measure.angles( self );
        self->measure.distance( self );
        self->update.angles( self );
        self->update.distance( self );
        avg_roll  += self->states.roll [ 1 ];
        avg_pitch += self->states.pitch[ 1 ];
        avg_z     += self->states.z    [ 1 ];

        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    }
    avg_roll  /= N_MEASURES;                                                        /* Calculate average value for Roll */
    avg_pitch /= N_MEASURES;                                                        /* Calculate average value for Pitch */
    avg_z     /= N_MEASURES;                                                        /* Calculate average value for z */
    self->sp.roll  = avg_roll;                             /* Update Roll SetPoint */
    self->sp.pitch = avg_pitch;                            /* Update Pitch SetPoint */
    self->sp.z     = avg_z;                                /* Update z SetPoint */
    printf( "Calibration done.\n" );
}


esp_err_t Dron( dron_t * self, dron_cfg_t * dron_cfg )
{
    for (int i = 0; i < 2; i++)
    {
        self->states.z        [ i ] = 0;
        self->states.roll     [ i ] = 0;
        self->states.pitch    [ i ] = 0;
        self->states.yaw      [ i ] = 0;
        self->states.roll_dot [ i ] = 0;
        self->states.pitch_dot[ i ] = 0;
        self->states.yaw_dot  [ i ] = 0;
    }

    self->sp.z     = 0;
    self->sp.roll  = 0;
    self->sp.pitch = 0;
    self->sp.yaw   = 0;

    self->measure.distance = measure_distance;
    self->measure.angles   = measure_angles;
    self->update.distance  = update_distance;
    self->update.angles    = update_angles;

    if( Hcsr04( &( self->components.hcsr04 ),                                                                           /* Make an instance of Hcsr04 class */
        dron_cfg->hcsr04_cfg.trig,
        dron_cfg->hcsr04_cfg.echo
    ) != ESP_OK )
        return ESP_FAIL;

    if( Mpu9250( &( self->components.mpu ),                                                                             /* Make an instance of Mpu9250 class */
        dron_cfg->mpu9250_cfg.i2c_cfg.address, dron_cfg->mpu9250_cfg.i2c_cfg.scl, dron_cfg->mpu9250_cfg.i2c_cfg.sda,    /* i2c parameters */
        dron_cfg->mpu9250_cfg.acc_range,                                                                                /* Accelerometer parameter */
        dron_cfg->mpu9250_cfg.gyro_range,                                                                               /* Gyroscope parameter */
        dron_cfg->mpu9250_cfg.ak8963_mode, dron_cfg->mpu9250_cfg.ak8963_bit, dron_cfg->mpu9250_cfg.mag_declination      /* Magnetometer parameters */
    ) != ESP_OK )
        return ESP_FAIL;
    
    self->components.mpu.Gyro.offset.x = dron_cfg->mpu9250_cfg.gyro_offset.x;                                           /* Set gyroscope x offset */
    self->components.mpu.Gyro.offset.y = dron_cfg->mpu9250_cfg.gyro_offset.y;                                           /* Set gyroscope y offset */
    self->components.mpu.Gyro.offset.z = dron_cfg->mpu9250_cfg.gyro_offset.z;                                           /* Set gyroscope z offset */

    if( Pwm( &( self->components.pwm[ m1 ] ), &( dron_cfg->pwm_cfg.m1 ) ) != ESP_OK )                                   /* Make an instance of Pwm class for Motor 1 */
        return ESP_FAIL;
    if( Pwm( &( self->components.pwm[ m2 ] ), &( dron_cfg->pwm_cfg.m2 ) ) != ESP_OK )                                   /* Make an instance of Pwm class for Motor 2 */
        return ESP_FAIL;
    if( Pwm( &( self->components.pwm[ m3 ] ), &( dron_cfg->pwm_cfg.m3 ) ) != ESP_OK )                                   /* Make an instance of Pwm class for Motor 3 */
        return ESP_FAIL;
    if( Pwm( &( self->components.pwm[ m4 ] ), &( dron_cfg->pwm_cfg.m4 ) ) != ESP_OK )                                   /* Make an instance of Pwm class for Motor 4 */
        return ESP_FAIL;

    Spiffs( &( self->components.spiffs ),                               /* Make an instance of Spiffs class */
        &( dron_cfg->spiffs_cfg.cfg ),                                  /* Set spiffs configurations */
        dron_cfg->spiffs_cfg.filenames,                                 /* Set filenames of files to be stored in Flash memory */
        dron_cfg->spiffs_cfg.n_files                                    /* Set Total of files to be stored in Flash memory */
    );

    Pid( &( self->components.controller[ z ] ),                         /* Make an instance of Pid class for z */
        dron_cfg->pid_controller_cfg.z_gains.kp,                        /* Set z Kp Gain */
        dron_cfg->pid_controller_cfg.z_gains.ki,                        /* Set z Ki Gain */
        dron_cfg->pid_controller_cfg.z_gains.kd                         /* Set z Kd Gain */
    );

    Pid( &( self->components.controller[ roll ] ),                      /* Make an instance of Pid class for Roll */
        dron_cfg->pid_controller_cfg.roll_gains.kp,                     /* Set Roll Kp Gain */
        dron_cfg->pid_controller_cfg.roll_gains.ki,                     /* Set Roll Ki Gain */
        dron_cfg->pid_controller_cfg.roll_gains.kd                      /* Set Roll Kd Gain */
    );

    Pid( &( self->components.controller[ pitch ] ),                     /* Make an instance of Pid class for Pitch */
        dron_cfg->pid_controller_cfg.pitch_gains.kp,                    /* Set Pitch Kp Gain */
        dron_cfg->pid_controller_cfg.pitch_gains.ki,                    /* Set Pitch Ki Gain */
        dron_cfg->pid_controller_cfg.pitch_gains.kd                     /* Set Pitch Kd Gain */
    );

    Pid( &( self->components.controller[ yaw ] ),                       /* Make an instance of Pid class for Yaw */
        dron_cfg->pid_controller_cfg.yaw_gains.kp,                      /* Set Yaw Kp Gain */
        dron_cfg->pid_controller_cfg.yaw_gains.ki,                      /* Set Yaw Ki Gain */
        dron_cfg->pid_controller_cfg.yaw_gains.kd                       /* Set Yaw Kd Gain */
    );

    Pid( &( self->components.controller[ roll_dot ] ),                  /* Make an instance of Pid class for Roll */
        dron_cfg->pid_controller_cfg.roll_dot_gains.kp,                 /* Set Roll Kp Gain */
        dron_cfg->pid_controller_cfg.roll_dot_gains.ki,                 /* Set Roll Ki Gain */
        dron_cfg->pid_controller_cfg.roll_dot_gains.kd                  /* Set Roll Kd Gain */
    );

    Pid( &( self->components.controller[ pitch_dot ] ),                 /* Make an instance of Pid class for Pitch */
        dron_cfg->pid_controller_cfg.pitch_dot_gains.kp,                /* Set Pitch Kp Gain */
        dron_cfg->pid_controller_cfg.pitch_dot_gains.ki,                /* Set Pitch Ki Gain */
        dron_cfg->pid_controller_cfg.pitch_dot_gains.kd                 /* Set Pitch Kd Gain */
    );

    Pid( &( self->components.controller[ yaw_dot ] ),                   /* Make an instance of Pid class for Yaw */
        dron_cfg->pid_controller_cfg.yaw_dot_gains.kp,                  /* Set Yaw Kp Gain */
        dron_cfg->pid_controller_cfg.yaw_dot_gains.ki,                  /* Set Yaw Ki Gain */
        dron_cfg->pid_controller_cfg.yaw_dot_gains.kd                   /* Set Yaw Kd Gain */
    );

    Bt_evt( &( self->components.bt ),                                                                                   /* Make an instance of Bt_evt class */
        dron_cfg->bt_cfg.new_device,
        dron_cfg->bt_cfg.load_bytes,
        dron_cfg->bt_cfg.erase_bytes
    );

    if( Joystick( &( self->components.flight_controller ), dron_cfg->flight_controller_mac ) != ESP_OK )                /* Make an instance of Joystick class */
        return ESP_FAIL;

    Mma( &self->components.mma );                                                                                       /* Make an instance of Mma class */
    
    dron_calibration( self );                                                                                           /* Calibrate Roll and Pitch SetPoints */

    gpio_set_level( 2, 0 );
    vTaskDelay( pdMS_TO_TICKS( 1000 ) );                                                                                /* Turn LED OFF and ON to indicate Dron is initialized */
    gpio_set_level( 2, 1 );
    printf( "Dron initialized\n" );

    return ESP_OK;
}
