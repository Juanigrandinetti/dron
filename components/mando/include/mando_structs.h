#ifndef STRUCT_MANDO
#define STRUCT_MANDO



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <mac_address.h>



/* *****************************************
                OBJECT STRUCT
   ***************************************** */

typedef struct joystick
{
    /*
    * Attributes
    * ----------
    * 
    * */
    uint8_t mac[ MAC_ADDR_SIZE ];
}joystick_t;

#endif
