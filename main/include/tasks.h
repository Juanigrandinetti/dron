#ifndef DRON_TASKS
#define DRON_TASKS



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <stdio.h>
#include <dron.h>

#define CORE_0          0              /* Core 0 */
#define CORE_1          1              /* Core 1 */


/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief FreeRTOS Task to check if flight controller sends a command in last 5 seconds.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
//void vTaskCheckCmdFlag( void * pvParameters );

/**
 * @brief FreeRTOS Task to update Duty Cycle of each motor.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
void vTaskUpdateDC( void * pvParameters );

/**
 * @brief FreeRTOS Task to Update Mma object output signals.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
void vTaskUpdateMMA( void * pvParameters );

/**
 * @brief FreeRTOS Task calculate PID Controller z action.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
void vTaskPIDz( void * pvParameters );

/**
 * @brief FreeRTOS Task calculate PID Controller Roll action.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
void vTaskPIDRoll( void * pvParameters );

/**
 * @brief FreeRTOS Task calculate PID Controller Pitch action.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
void vTaskPIDPitch( void * pvParameters );

/**
 * @brief FreeRTOS Task calculate PID Controller Yaw action.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
void vTaskPIDYaw( void * pvParameters );

/**
 * @brief Handle Bluetooth Commands.
 * @param data: Pointer to char.
 * @retval none
 */
static void parse_bt_cmd( char * data );

/**
 * @brief Task to handle Bluetooth Flags of ps3_spp.c file.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
void vTaskHandleBtFlags( void * pvParameters );

/**
* @brief Task to print distance.
* @param pvParameters: Pointer to void.
* @retval none
*/
void vTaskPrintStates( void * pvParameters );

/**
* @brief Task to update distance.
* @param pvParameters: Pointer to void.
* @retval none
*/
void vTaskUpdateDistance( void * pvParameters );

/**
 * 
 */
void vTaskMeasureDistance( void * pvParameters );

/**
 * 
 */
void vTaskUpdateAngles( void * pvParameters );

/**
 * 
 */
void vTaskMeasureAngles( void * pvParameters );

/**
 * @brief FreeRTOS Task to extract bytes from Flash memory.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
static void vTaskExctractFromFlash( void * pvParameters );

/**
 * @brief FreeRTOS Task to load bytes to Flash memory.
 * @param pvParameters: Pointer to void.1
 * @retval none
 */
static void vTaskLoadToFlash( void * pvParameters );

/**
 * @brief FreeRTOS Task to update PID Controller gains.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
static void vTaskUpdatePIDParameters( void * pvParameters );

/**
 * @brief FreeRTOS Task to erase bytes from Flash memory.
 * @param pvParameters: Pointer to void.
 * @retval none
 */
static void vTaskEraseFlash( void * pvParameters );

#endif
