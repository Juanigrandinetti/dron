#ifndef SPIFFS_PARTITION
#define SPIFFS_PARTITION



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <spiffs_partition_structs.h>



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief Make instance of Spiffs class.
 * @param self: Pointer to spiffs_t struct.
 * @param cfg_p: esp_vfs_spiffs_conf_t struct with spiffs configurations.
 * @param filenames_p: Array containing the names of the files that will be stored in Spiffs partition.
 * @param n_files_p: Total of files to be stored in Spiffs partition.
 * @retval none
 */
void Spiffs( spiffs_t * self, esp_vfs_spiffs_conf_t * cfg_p, char * filenames_p[], int n_files_p );

/**
 * @brief Init Spiffs object.
 * @param self: Pointer to spiffs_t struct.
 * @retval none
 */
void spiffs_init( spiffs_t * self );

#endif
