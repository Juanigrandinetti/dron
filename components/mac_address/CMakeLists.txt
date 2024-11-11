idf_component_register(SRCS "mac_address.c" "mac_address_structs.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_wifi" "nvs_flash")
