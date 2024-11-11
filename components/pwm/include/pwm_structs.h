#ifndef STRUCT_PWM
#define STRUCT_PWM


/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <driver/mcpwm.h>



/* ********************************************
                    DEFINES
   ******************************************** */

#define MAX_DC          10                              /* Maximum Duty Cycle ( maximum motor speed ). */
#define MIN_DC          5                               /* Mínimo Duty Cylce ( motor shut down ). */



/* *****************************************
                OBJECT STRUCT
   ***************************************** */

typedef enum motors
{
   m1,      /* Motor 1 index used in Arrays */
   m2,      /* Motor 2 index used in Arrays */
   m3,      /* Motor 3 index used in Arrays */
   m4,      /* Motor 4 index used in Arrays */
}motors_e;

/**
* @brief pwm struct.
* @param gpio: pwm GPIO.
* @param unit: pwm unit ( 0/1 ).
* @param timer: pwm Timer ( 0/1/2 ).
* @param gen: pwm Generator ( A/B ).
* @param cfg: pwm configurations.
* @param min_dc: Minimum Duty Cycle.
* @param max_dc: Maximum Duty Cycle.
* @brief
*/
typedef struct pwm_cfg
{
   int gpio;
   mcpwm_unit_t unit;
   mcpwm_timer_t timer;
   mcpwm_generator_t gen;
   mcpwm_config_t mcpwm_cfg;
   float min_dc;
   float max_dc;
}pwm_cfg_t;

/**
* @brief pwm struct.
* @param cfg: .
* @param increase_dc: Pointer to increase_dc function.
* @param decrease_dc: Pointer to decrease_dc function.
*/
typedef struct pwm
{
   /*
   * Attributes
   * ----------
   * 
   * */
   pwm_cfg_t cfg;

   /*
   * Methods
   * -------
   * 
   * */

   void ( *increase_dc )( struct pwm_t * self );
   void ( *decrease_dc )( struct pwm_t * self );

   /**
    * @brief Get Duty Cycle of pwm signal.
    * @param self: Pointer to pwm_t struct.
    * @retval float
    */
   float ( *get_dc )( struct pwm_t * self );

   /**
    * @brief Set Duty Cycle of pwm signal.
    * @param self: Pointer to pwm_t struct.
    * @param dc: Duty Cycle to be set.
    * @retval none
    */
   void ( *set_dc )( struct pwm_t * self, float dc );
}pwm_t;



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
* @brief Increase Duty Cycle of pwm.
* @param self: Pointer to pwm_t structure.
* @retval none
*/
void increase_dc( pwm_t * self );

/**
* @brief Decrease Duty Cycle of pwm.
* @param self: Pointer to pwm_t structure.
* @retval none
*/
void decrease_dc( pwm_t * self );

/**
 * @brief Set Duty Cycle of pwm signal.
 * @param self: Pointer to pwm_t struct.
 * @param dc: Duty Cycle to be set.
 * @retval none
 */
void set_dc( pwm_t * self, float dc );

/**
 * @brief Get Duty Cycle of pwm signal.
 * @param self: Pointer to pwm_t struct.
 * @retval float
 */
float get_dc( pwm_t * self );

#endif
