#ifndef HCSR04
#define HCSR04

/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <driver/gpio.h>
#include <hcsr04_structs.h>



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
* @brief Make an instance of Hcsr04 class.
* @param self: Pointer to hcsr04_t struct.
* @param trig_p: Trigger pin.
* @param echo_p: Echo pin.
* @retval esp_err_t
*/
esp_err_t Hcsr04( hcsr04_t * self, int trig_p, int echo_p );

/**
* @brief Initialize Hcsr04 object.
* @param self: Pointer to hcsr04_t struct.
* @retval esp_err_t
*/
static esp_err_t hcsr04_init( hcsr04_t * self );

#endif
