idf_component_register(SRCS "bt_events.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "spiffs_partition" "driver" "quadcopter_controller")
