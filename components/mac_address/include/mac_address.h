#ifndef MAC_ADDRESS
#define MAC_ADDRESS



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <mac_address_structs.h>
#include <nvs_flash.h>

/**
* @brief Make an instance of Mac class.
* @param self: Pointer to mac_address_t struct.
* @param mac_p: Array with Joystick's MAC Address.
* @retval none
*/
void Mac( mac_address_t * self, const uint8_t mac_p[ MAC_ADDR_SIZE ] );

/**
* @brief Initialize NVS.
* @param none
* @retval none
*/
void nvs_init( void );

#endif
