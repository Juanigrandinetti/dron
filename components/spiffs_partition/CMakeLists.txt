idf_component_register(SRCS "spiffs_partition.c" "spiffs_partition_structs.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "spiffs" "esp_partition" "esp_system")
