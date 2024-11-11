#ifndef DRON_CFGS
#define DRON_CFGS



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <dron_structs.h>



/* **********************************************
                VARIABLES DECLARATION
   ********************************************** */

const uint8_t mac_joystick[ MAC_ADDR_SIZE ] = { 0xF0, 0xF0, 0x02, 0x43, 0x53, 0x53 };       /* Joystick's MAC Address */                                                          
char * filenames[] = { "/storage/valores_medidos.txt" };                                    /* Pointer to array of pointers ( char * ) */
dron_cfg_t dron_cfg;                                                                        /* Used to configure Dron object */

#endif
