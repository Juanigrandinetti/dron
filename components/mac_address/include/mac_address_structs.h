#ifndef STRUCT_MAC_ADDRESS
#define STRUCT_MAC_ADDRESS

/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <esp_system.h>
#include <esp_wifi.h>
#include <esp_event.h>



/* ********************************************
                    DEFINES
   ******************************************** */

#define MAC_ADDR_SIZE              6                       /* Amount of bytes of any MAC Address */



/* *****************************************
                OBJECT STRUCT
   ***************************************** */

/**
* @brief mac_address_t struct.
* @param mac: MAC Address
* @param get_mac_address: Pointer to get_mac_address function.
* @param set_mac_address: Pointer to set_mac_address function.
* @param nvs_init: Pointer to nvs_init function.
*/
typedef struct mac_address
{
   /*
   * Attributes
   * ----------
   * 
   * */
   uint8_t mac[ MAC_ADDR_SIZE ];

   /*
   * Methods
   * -------
   * 
   * */
   void ( *get_mac_address )( struct mac_address_t * self );
   esp_err_t ( *set_mac_address )( struct mac_address_t * self, const uint8_t * new_mac );
}mac_address_t;

/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
* @brief Get MAC Address.
* @param self: Pointer to mac_address_t struct.
* @retval none
*/
void get_mac_address( mac_address_t* self );

/**
 * @brief Set MAC Address.
 * @param self: Pointer to mac_address_t struct.
 * @param new_mac: New MAC Address to assign to ESP.
 * @retval esp_err_t
*/
esp_err_t set_mac_address( mac_address_t * self, const uint8_t * new_mac );

#endif
