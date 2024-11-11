#include <mac_address_structs.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void get_mac_address( mac_address_t * self )
{
    esp_wifi_get_mac( ESP_IF_WIFI_STA, self->mac );
    printf( "Original ESP MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
        self->mac[ 0 ], self->mac[ 1 ], self->mac[ 2 ], self->mac[ 3 ], self->mac[ 4 ], self->mac[ 5 ] );
}

esp_err_t set_mac_address( mac_address_t * self, const uint8_t * new_mac )
{
    for (size_t i = 0; i < MAC_ADDR_SIZE; i++)
        self->mac[ i ] = new_mac[ i ];
    esp_err_t res = esp_wifi_set_mac( ESP_IF_WIFI_STA, self->mac );
    if( res != ESP_OK )
        return ESP_FAIL;
    else
    {
        fflush( stdout );
        printf( "New ESP MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
            self->mac[ 0 ], self->mac[ 1 ], self->mac[ 2 ], self->mac[ 3 ], self->mac[ 4 ], self->mac[ 5 ] );
    }
    
    return ESP_OK;
}
