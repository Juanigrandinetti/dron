idf_component_register(SRCS "dron_structs.c" "dron_cfgs.c" "dron.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "hcsr04" "mpu9250" "spiffs_partition" "quadcopter_controller"
                    "bt_events" "pwm" "mando" "motor_mixing_algorithm")
