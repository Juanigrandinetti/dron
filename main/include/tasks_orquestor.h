#ifndef TASKS_ORQUESTOR
#define TASKS_ORQUESTOR

#include <tasks.h>

typedef enum core
{
    core_0,
    core_1,
}core_e;

/**
 * @brief
 * @param flag_p: Pointer to bt_flags_t struct.
 * @param core_p: core_p: CPU Core which will run related tasks.
 * @retval none
 */
void vTaskBluetooth( bt_flags_t * flag_p, core_e core_p );

/**
* @brief Initialize FreeRTOS tasks.
* @param core_p: CPU Core which will run related tasks.
* @retval none
*/
void vTaskMeasures( core_e core_p );

/**
 * @brief Initialize FreeRTOS Controllers related Tasks.
 * @param core_p: CPU Core which will run related tasks.
 * @retval none
 */
void vTaskControllers( core_e core_p );

#endif
