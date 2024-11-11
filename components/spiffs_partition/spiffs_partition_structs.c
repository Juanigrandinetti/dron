#include <spiffs_partition.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void set_cfg( spiffs_t * self )
{
    esp_err_t res = esp_vfs_spiffs_register( &self->cfg );
    if( res != ESP_OK )
    {
        switch ( res )
        {
        case ESP_ERR_NO_MEM:
            printf( "[SPIFFS] Not enough memory to mount spiffs partition.\n" );
            break;
        case ESP_ERR_INVALID_STATE:
            printf( "[SPIFFS] spiffs partition encrypted.\n" );
            break;
        case ESP_ERR_NOT_FOUND:
            printf( "[SPIFFS] Partition not found.\n" );
            break;
        case ESP_FAIL:
            printf( "[SPIFFS] Failed to configurate or format spiffs partition.\n" );
            break;
        default:
            printf( "[SPIFFS] Failed to mount spiffs partition ( %s )\n", esp_err_to_name( res ) );
            break;
        }
        esp_restart();
    }
}

void info( spiffs_t * self )
{
    esp_err_t res = esp_spiffs_info( self->cfg.partition_label, &self->total, &self->used );
    if( res != ESP_OK )
        printf( "[SPIFFS] Error to obtain spiffs info ( %s )\n", esp_err_to_name( res ) );
    else
        printf( "spiffs size: %d\nUsed bytes: %d\n", self->total, self->used );
}

const esp_partition_t * get_partition( spiffs_t * self )
{
    const esp_partition_t * partition = esp_partition_find_first( ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, self->cfg.partition_label );
    
    return partition;
}

esp_err_t erase_partition( spiffs_t * self, const esp_partition_t * partition, size_t offset, size_t spiffs_size )
{
    esp_err_t res = esp_partition_erase_range( partition, offset, spiffs_size );
    if( res != ESP_OK )
        return res;
    
    return ESP_OK;
}
