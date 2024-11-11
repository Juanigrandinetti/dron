#ifndef STRUCT_BT_EVENTS
#define STRUCT_BT_EVENTS



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <spiffs_partition.h>
#include <quadcopter_controller.h>



/* ********************************************
                    DEFINES
   ******************************************** */

#define     BT_TASK_STACK     1024           /* Unit of memory used for Bluetooth tasks */



/* *****************************************
                OBJECT STRUCT
   ***************************************** */

/**
 * @brief bt_evt_t struct used to manage Bluetooth connection between esp and device ( e.g smartphone ).
 * @param state: State variable ( z/roll/pitch/yaw ).
 * @param pid_type: Type of PID action ( p: Proportional, i: Integral, d: Derivative ).
 * @param value: New value to assign.
 * @param new_device_gpio: LED GPIO used when new device is connected.
 * @param load_gpio: LED GPIO used when loading bytes to Flash memory.
 * @param erase_gpio: LED GPIO used when erasing bytes from Flash memory.
 */
typedef struct bt_evt
{
   int state;                 /* State variable ( z/roll/pitch/yaw ) */
   char * pid_type;           /* Type of PID action ( p: Proportional, i: Integral, d: Derivative ) */
   char * value;              /* New value to assign */
   int new_device_gpio;       /* LED GPIO used when new device is connected */
   int load_gpio;             /* LED GPIO used when loading bytes to Flash memory */
   int erase_gpio;            /* LED GPIO used when erasing bytes from Flash memory */
}bt_evt_t;


/**
 * @brief task_params_t struct used to link Bluetooth events and dron objects.
 * @param bt: bt_evt_t struct used to manage Bluetooth events.
 * @param spiffs: spiffs_t struct used to access Flash memory.
 * @param pid_controller: pid_controller_t struct used to modify PID Controllers gains.
 */
typedef struct task_params
{
   bt_evt_t bt;
   spiffs_t spiffs;
   pid_controller_t (* pid_controller )[ 4 ];
}task_params_t;

#endif
