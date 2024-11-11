#ifndef BT_EVENTS
#define BT_EVENTS



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <bt_events_structs.h>



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief Initialize Bt_evt object.
 * @param self: Pointer to bt_evt_t struct.
 * @retval none
 */
void bt_init( bt_evt_t * self );

/**
 * @brief Make an instance of Bt_evt class.
 * @param self: Pointer to bt_evt_t struct.
 * @param new_device_gpio_p: New device related GPIO ( connected to a LED ).
 * @param load_gpio_p: Load bytes to Flash memory related GPIO ( connected to a LED ).
 * @param erase_gpio_p: Erase bytes from Flash memory related GPIO ( connected to a LED ).
 */
void Bt_evt( bt_evt_t * self, int new_device_gpio_p, int load_gpio_p, int erase_gpio_p );

#endif
