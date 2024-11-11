#ifndef BT_INT
#define BT_INT



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <stdbool.h>



/* *****************************************
                    STRUCTS
   ***************************************** */

/**
 * @brief bt_flags_t struct used to handle Bluetooth events in main application.
 * @param new_dev: New Device has connected.
 * @param data_received: Data received.
 * @param data: Array of char to store bytes received.
 */
typedef struct bt_flags
{
    bool new_dev;               /* New Device has connected */
    bool data_received;         /* Data received */
    char data[ 256 ];           /* Array of char to store bytes received */
}bt_flags_t;

#endif
