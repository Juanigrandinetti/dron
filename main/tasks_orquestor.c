#include <tasks_orquestor.h>

void vTaskBluetooth( bt_flags_t * flag_p, core_e core_p )
{
    xTaskCreatePinnedToCore(                    /* Create FreeRTOS Task to handle Bluetooth events */
        vTaskHandleBtFlags,
        "vTaskHandleBtFlags",
        BT_TASK_STACK * 2,
        ( void * ) flag_p,
        1,
        NULL,
        core_p
    );
}

void vTaskControllers( core_e core_p )
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandlePIDz     = NULL;
    TaskHandle_t xHandlePIDRoll  = NULL;
    TaskHandle_t xHandlePIDPitch = NULL;
    TaskHandle_t xHandlePIDYaw   = NULL;
    TaskHandle_t xHandleMMA        = NULL;
    TaskHandle_t xHandleDutyCycle  = NULL;

    xTaskCreatePinnedToCore(
        vTaskPIDz,
        "vTaskPIDz",
        1024 * 2,
        &ucParameterToPass,
        2,
        &xHandlePIDz,
        core_p
    );

    /*xTaskCreatePinnedToCore(
        vTaskPIDRoll,
        "vTaskPIDRoll",
        1024 * 2,
        &ucParameterToPass,
        2,
        &xHandlePIDRoll,
        core_p
    );

    xTaskCreatePinnedToCore(
        vTaskPIDPitch,
        "vTaskPIDPitch",
        1024 * 2,
        &ucParameterToPass,
        2,
        &xHandlePIDPitch,
        core_p
    );

    xTaskCreatePinnedToCore(
        vTaskPIDYaw,
        "vTaskPIDYaw",
        1024 * 2,
        &ucParameterToPass,
        2,
        &xHandlePIDYaw,
        core_p
    );*/

    xTaskCreatePinnedToCore(
        vTaskUpdateMMA,
        "vTaskUpdateMMA",
        1024 * 2,
        &ucParameterToPass,
        2,
        &xHandleMMA,
        core_p
    );

    xTaskCreatePinnedToCore(
        vTaskUpdateDC,
        "vTaskUpdateDC",
        1024 * 2,
        &ucParameterToPass,
        1,
        &xHandleDutyCycle,
        core_p
    );
}

/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void vTaskMeasures( core_e core_p )
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    xTaskCreatePinnedToCore(
        vTaskMeasureDistance,
        "vTaskMeasureDistance",
        1024 * 2,
        &ucParameterToPass,
        1,
        &xHandle,
        core_p
    );

    xTaskCreatePinnedToCore(
        vTaskUpdateDistance,
        "vTaskUpdateDistance",
        1024 * 2,
        &ucParameterToPass,
        1,
        &xHandle,
        core_p
    );

    xTaskCreatePinnedToCore(
        vTaskPrintStates,
        "vTaskPrintStates",
        1024 * 2,
        &ucParameterToPass,
        1,
        &xHandle,
        core_p
    );
    
    xTaskCreatePinnedToCore(
        vTaskMeasureAngles,
        "vTaskMeasureAngles",
        1024 * 2,
        &ucParameterToPass,
        1,
        &xHandle,
        core_p
    );

    xTaskCreatePinnedToCore(
        vTaskUpdateAngles,
        "vTaskUpdateAngles",
        1024 * 2,
        &ucParameterToPass,
        1,
        &xHandle,
        core_p
    );
}


