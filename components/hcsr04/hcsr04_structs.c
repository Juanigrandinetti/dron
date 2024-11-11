#include <hcsr04_structs.h>
#include <rom/ets_sys.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

esp_err_t hcsr04_measure( hcsr04_t * self )
{
    /** IMPORTANT!!
     *
     * - Do not add prints nor LOGs cause it could cause a wrong measure
     * - Do not add interrupts
     * 
     * 
     */
    
    taskENTER_CRITICAL( &spinlock );                                    /* Disable interrupts => Only this task can access this shared resource */
    
    gpio_set_level( self->trig, 0 );                                    /* Routine to send 40KHz ultrasonic wave */
    ets_delay_us( LOW_TRIG_DELAY );
    gpio_set_level( self->trig, 1);
    ets_delay_us( HIGH_TRIG_DELAY );
    gpio_set_level( self->trig, 0 );

    if( gpio_get_level( self->echo ) )                                  /* Check if previous ultrasonic wave has returned */
        return ESP_ERR_INVALID_STATE;

    int64_t start = esp_timer_get_time();
    while ( !gpio_get_level( self->echo ) )                             /* Wait until time ( echo ) pulse has started */
    {
        if( esp_timer_get_time() - start > ECHO_TIMEOUT )               /* Check if actual waiting time has exceed maximum waiting time */
            return ESP_ERR_TIMEOUT;
    }

    int64_t echo_start = esp_timer_get_time();
    int64_t echo_end   = echo_start;
    while( gpio_get_level( self->echo ) )                               /* Once ultrasonic wave was sent, wait until it returns */
    {
        echo_end = esp_timer_get_time();
        if( echo_end - echo_start > MEASUREMENT_TIMEOUT )               /* Check if actual waiting measurement time has exceed maximum waiting time */
            self->time = -1;                                            /* If so, activate flag */
    }

    self->time = echo_end - echo_start;                                 /* Update time attribute of Hcsr04 object */
    
    taskEXIT_CRITICAL( &spinlock );                                     /* Enable interrupts again */

    return ESP_OK;
}
