#include <stdio.h>
#include <dron.h>
#include <tasks_orquestor.h>



/* ********************************************
                GLOBAL VARIABLES
   ******************************************** */

dron_t dron;                                        /* Used to make an instance of Dron class */



/* ***************************************
                MAIN PROGRAM
   *************************************** */

void app_main( void )
{
    extern bt_flags_t flag;                         /* Declared in ps3/src/include/bt_int.h and defined in ps3_spp.c */
    extern dron_cfg_t dron_cfg;                     /* Declared in dron_cfgs.h and defined in dron_cfgs.c */
    
    if( Dron( &dron, &dron_cfg ) != ESP_OK )                    
        return;

    vTaskBluetooth( &flag, core_0 );                /* Initialize Bluetooth related FreeRTOS Tasks */
    vTaskMeasures( core_0 );                        /* Initialize Dron's related FreeRTOS Tasks */
    vTaskControllers( core_1 );                     /* Initialize Controllers related FreeRTOS Tasks */

    return;
}
