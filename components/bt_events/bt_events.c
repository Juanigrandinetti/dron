#include <stdio.h>
#include <bt_events.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void bt_init( bt_evt_t * self )
{
    gpio_reset_pin( self->new_device_gpio );                                        /* Initialize GPIO's related LED's */
    gpio_set_direction( self->new_device_gpio, GPIO_MODE_OUTPUT );

    gpio_reset_pin( self->load_gpio );
    gpio_set_direction( self->load_gpio, GPIO_MODE_OUTPUT );

    gpio_reset_pin( self->erase_gpio );
    gpio_set_direction( self->erase_gpio, GPIO_MODE_OUTPUT );
    
    printf( "Bluetooth events initialized.\n" );
}

void Bt_evt( bt_evt_t * self, int new_device_gpio_p, int load_gpio_p, int erase_gpio_p )
{
    self->state           = 0;
    self->pid_type        = "";
    self->value           = 0;
    self->new_device_gpio = new_device_gpio_p;              /* Initializing attributes and assignment of parameters to those which correspond */
    self->load_gpio       = load_gpio_p;
    self->erase_gpio      = erase_gpio_p;
    bt_init( self );                                        /* Initialize object */
}
