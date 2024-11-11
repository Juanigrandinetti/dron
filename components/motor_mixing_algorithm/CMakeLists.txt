idf_component_register(SRCS "motor_mixing_algorithm.c" "motor_mixing_algorithm_structs.C"
                    INCLUDE_DIRS "include"
                    REQUIRES "quadcopter_controller" "pwm")
