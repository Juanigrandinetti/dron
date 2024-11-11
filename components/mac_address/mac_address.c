#include <stdio.h>
#include <mac_address.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void Mac( mac_address_t * self, const uint8_t mac_p[ MAC_ADDR_SIZE ] )
{
    for (size_t i = 0; i < MAC_ADDR_SIZE; i++)              /* Assignment of parameters to object attributes */
        self->mac[ i ] = mac_p[ i ];
    self->get_mac_address = get_mac_address;                /* Pointer assignment to get_mac_address function */
    self->set_mac_address = set_mac_address;                /* Pointer assignment to set_mac_address function */
    nvs_init();                                             /* Initialize NVS */
}

void nvs_init( void )
{
    esp_err_t res = nvs_flash_init();
    if ( res == ESP_ERR_NVS_NO_FREE_PAGES || res == ESP_ERR_NVS_NEW_VERSION_FOUND )
	{
		ESP_ERROR_CHECK( nvs_flash_erase() );
		res = nvs_flash_init();
	}
	ESP_ERROR_CHECK( res );
    ESP_ERROR_CHECK( esp_netif_init() );
    ESP_ERROR_CHECK( esp_event_loop_create_default() );
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init( &cfg ) );
    ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}