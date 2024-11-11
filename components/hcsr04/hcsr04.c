#include <stdio.h>
#include <hcsr04.h>
#include <freertos/FreeRTOS.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

esp_err_t Hcsr04( hcsr04_t * self, int trig_p, int echo_p )
{
    self->trig = trig_p;                    /* Assignment of Trig and Echo pins to Hcsr04 object attributes */
    self->echo = echo_p;
    self->time = 0;                         /* Initialize Echo duration time to 0 */
    self->measure  = hcsr04_measure;        /* Pointer assignment to hcsr04_measure function */
    
    esp_err_t res = hcsr04_init( self );    /* Initialize Hcsr04 object */

    return res;
}

static esp_err_t hcsr04_init( hcsr04_t * self )
{
    gpio_reset_pin( self->trig );                                   /* Initialize Trig GPIO */
    gpio_set_direction( self->trig, GPIO_MODE_OUTPUT );

    gpio_reset_pin( self->echo );                                   /* Initialize Echo GPIO */
    gpio_set_direction( self->echo, GPIO_MODE_INPUT );

    return ESP_OK;
}
