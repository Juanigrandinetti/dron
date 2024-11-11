#ifndef PWM
#define PWM

/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <pwm_structs.h>



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
* @brief Make an instance of Pwm class.
* @param self: Pointer to pwm_t structure.
* @param pwm_cfg_p: Pointer to pwm_cfg_t struct used to configure Pwm object.
* @retval esp_err_t
*/
esp_err_t Pwm( pwm_t * self, pwm_cfg_t * pwm_cfg_p );

/**
* @brief Initialize Pwm object.
* @param self: Pointer to pwm_t structure.
* @retval esp_err_t
*/
esp_err_t pwm_init( pwm_t * self );

#endif
