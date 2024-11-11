#ifndef STRUCT_SPIFFS_PARTITION
#define STRUCT_SPIFFS_PARTITION



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <esp_spiffs.h>
#include <esp_partition.h>
#include <esp_system.h>



/* *****************************************
                OBJECT STRUCT
   ***************************************** */

/**
 * @brief spiffs_t struct used to access Flash memory.
 * @param cfg: spiffs configuration.
 * @param total: Total bytes of spiffs partition.
 * @param used: Total bytes used of spiffs partition.
 * @param n_files: Total of files to be stored in Spiffs partition.
 * @param filename: Pointer to array of strings ( char * ), which contains the names of the files to be stored in Spiffs partition.
 * @param set_cfg: Set spiffs configurations.
 * @param info: Get information about spiffs partition.
 * @param get_partition: Get the actual spiffs partition.
 * @param erase_partition: Erase bytes starting from 'offset' parameter.
 */
typedef struct spiffs
{
   /*
   * Attributes
   * ----------
   * 
   */
   esp_vfs_spiffs_conf_t cfg;
   size_t total;
   size_t used;
   int n_files;
   char ** filename;

   /*
   * Methods
   * -------
   * 
   */
   void ( *set_cfg )( struct spiffs_t * self );
   void ( *info )( struct spiffs_t * self );
   const esp_partition_t * ( *get_partition )( struct spiffs_t * self );
   esp_err_t ( *erase_partition )( struct spiffs_t * self, const esp_partition_t * partition, size_t offset, size_t spiffs_size );
}spiffs_t;



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief Set spiffs configurations.
 * @param self: Pointer to spiffs_t struct.
 * @retval none
 */
void set_cfg( spiffs_t * self );

/**
 * @brief Get spiffs information.
 * @param self: Pointer to spiffs_t struct.
 * @retval none
 */
void info( spiffs_t * self );

/**
 * @brief Get spiffs actual partition.
 * @param self: Pointer to spiffs_t struct.
 * @retval none
 */
const esp_partition_t * get_partition( spiffs_t * self );

/**
 * @brief Erase bytes from spiffs partition starting from 'offset' parameter.
 * @param self: Pointer to spiffs_t struct.
 * @param partiton: Pointer to esp_partition_t struct.
 * @param offset: Memory address which defines where to start erasing bytes from spiffs partition.
 * @param spiffs_size: Total bytes of spiffs partition.
 * @retval none
 */
esp_err_t erase_partition( spiffs_t * self, const esp_partition_t * partition, size_t offset, size_t spiffs_size );

#endif
