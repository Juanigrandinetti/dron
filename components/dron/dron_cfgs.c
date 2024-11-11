#include <dron_cfgs.h>



/* ***********************************************
                VARIABLES DEFINITION
   *********************************************** */

dron_cfg_t dron_cfg = {
    .hcsr04_cfg  = {
        .trig = HCSR04_TRIG_GPIO,
        .echo = HCSR04_ECHO_GPIO
    },
    .mpu9250_cfg = {
        .i2c_cfg         = { .address = 0x68, .scl = SCL_GPIO, .sda = SDA_GPIO },
        .acc_range       = ACCELEROMETER_RANGE_2G, .gyro_range = GYRO_RANGE_250_DPS,
        .ak8963_mode     = AK8963_MODE_CONTINUOUS_8_HZ, .ak8963_bit = AK8963_BIT_16,
        .mag_declination = -9.8
    },
    .spiffs_cfg  = {
        .cfg = {
            .base_path              = "/storage",
            .partition_label        = "storage",
            .max_files              = 5,
            .format_if_mount_failed = true
        },
        .filenames = filenames,
        .n_files   = 1
    },
    .pid_controller_cfg = {
        .z_gains         = { .kp = Kp_z,         .kp_take_off = Kp_take_off_z, .ki = Ki_z,         .kd = Kd_z },
        .roll_gains      = { .kp = Kp_roll,      .kp_take_off = Kp_roll,       .ki = Ki_roll,      .kd = Kd_roll },
        .pitch_gains     = { .kp = Kp_pitch,     .kp_take_off = Kp_pitch,      .ki = Ki_pitch,     .kd = Kd_pitch },
        .yaw_gains       = { .kp = Kp_yaw,       .kp_take_off = Kp_yaw,        .ki = Ki_yaw,       .kd = Kd_yaw },
        .roll_dot_gains  = { .kp = Kp_roll_dot,  .kp_take_off = Kp_roll_dot,   .ki = Ki_roll_dot,  .kd = Kd_roll_dot },
        .pitch_dot_gains = { .kp = Kp_pitch_dot, .kp_take_off = Kp_pitch_dot,  .ki = Ki_pitch_dot, .kd = Kd_pitch_dot },
        .yaw_dot_gains   = { .kp = Kp_yaw_dot,   .kp_take_off = Kp_yaw_dot,    .ki = Ki_yaw_dot,   .kd = Kd_yaw_dot },
    },
    .bt_cfg = {
        .new_device  = NEW_DEV_CONN_GPIO,
        .load_bytes  = LOAD_BYTES_GPIO,
        .erase_bytes = ERASE_BYTES_GPIO
    },
    .flight_controller_mac = mac_joystick,
    .pwm_cfg = {
        .m1 = {
            .gpio      = PWM_M1_GPIO,
            .max_dc    = MAX_DC,
            .min_dc    = MIN_DC,
            .unit      = MCPWM_UNIT_0,
            .timer     = MCPWM_TIMER_0,
            .gen       = MCPWM_GEN_A,
            .mcpwm_cfg = {
                .frequency    = FREQUENCY,
                .cmpr_a       = MCPWM_GEN_A,
                .duty_mode    = MCPWM_DUTY_MODE_0,
                .counter_mode = MCPWM_UP_COUNTER
            }
        },
        .m2 = {
            .gpio      = PWM_M2_GPIO,
            .max_dc    = MAX_DC,
            .min_dc    = MIN_DC,
            .unit      = MCPWM_UNIT_0,
            .timer     = MCPWM_TIMER_1,
            .gen       = MCPWM_GEN_A,
            .mcpwm_cfg = {
                .frequency    = FREQUENCY,
                .cmpr_a       = MCPWM_GEN_B,
                .duty_mode    = MCPWM_DUTY_MODE_0,
                .counter_mode = MCPWM_UP_COUNTER
            }
        },
        .m3 = {
            .gpio      = PWM_M3_GPIO,
            .max_dc    = MAX_DC,
            .min_dc    = MIN_DC,
            .unit      = MCPWM_UNIT_0,
            .timer     = MCPWM_TIMER_0,
            .gen       = MCPWM_GEN_B,
            .mcpwm_cfg = {
                .frequency    = FREQUENCY,
                .cmpr_a       = MCPWM_GEN_A,
                .duty_mode    = MCPWM_DUTY_MODE_0,
                .counter_mode = MCPWM_UP_COUNTER
            }
        },
        .m4 = {
            .gpio      = PWM_M4_GPIO,
            .max_dc    = MAX_DC,
            .min_dc    = MIN_DC,
            .unit      = MCPWM_UNIT_0,
            .timer     = MCPWM_TIMER_1,
            .gen       = MCPWM_GEN_B,
            .mcpwm_cfg = {
                .frequency    = FREQUENCY,
                .cmpr_a       = MCPWM_GEN_B,
                .duty_mode    = MCPWM_DUTY_MODE_0,
                .counter_mode = MCPWM_UP_COUNTER
            }
        }
    }
};
