#ifndef MANDO
#define MANDO



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <ps3.h>
#include <bt_int.h>
#include <mando_structs.h>
#include <driver/gpio.h>



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
* @brief Make an instance of Joystick class.
* @param self: Pointer to joystick_t struct.
* @param mac: Array with Joystick's MAC Address.
* @retval esp_err_t
*/
esp_err_t Joystick( joystick_t * self, const uint8_t mac[ MAC_ADDR_SIZE ] );

/**
* @brief Manage Joystick's Callbacks.
* @param ps3: ps3_t struct.
* @param event: pse3_event_t struct.
* @retval none
*/
void controller_event_cb( ps3_t ps3, ps3_event_t event );

/**
* @brief Initialize Joystick. 
* @param self: Pointer to joystick_t struct.
* @retval esp_err_t
*/
esp_err_t joystick_init( joystick_t * self );

#endif
