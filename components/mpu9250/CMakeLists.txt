idf_component_register(SRCS "mpu9250.c" "communication.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "driver" "vfs")
