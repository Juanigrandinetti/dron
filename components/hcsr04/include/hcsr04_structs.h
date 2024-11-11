#ifndef STRUCT_HCSR04
#define STRUCT_HCSR04



/* ********************************************
                    DEFINES
   ******************************************** */

#define V_SONIDO                0.0343                      /* [cm/us] -------- (343 m/s) * (100 cm/m) / (1 s/1000000 us) = 0.03443 cm/us */
#define LOW_TRIG_DELAY          4                           /* [us] */
#define HIGH_TRIG_DELAY         10                          /* [us] */
#define ECHO_TIMEOUT            6000                        /* [ms] */
#define MEASUREMENT_TIMEOUT     13                          /* Maximum waiting time for a measurement ( Max. Distance is 4.459 meters ) */



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <driver/gpio.h>



/* *****************************************
                OBJECT STRUCT
   ***************************************** */

typedef struct hcsr04
{
   /**
    * Attributes
    * ----------
    * 
    */
   int trig;               /* Trig signal ( Tx ) */
   int echo;               /* Echo signal ( Rx ) */
   float time;             /* Duration of Echo signal */

   /**
    * Methods
    * -------
    * 
    */

   /**
    * @brief Measure duration of Echo signal of Hcsr04 object.
    * @param self: Pointer to hcsr04_t struct.
    * @retval esp_err_t
    */
   esp_err_t ( *measure )( struct hcsr04_t * self );
}hcsr04_t;



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief Measure duration of Echo signal of Hcsr04 object.
 * @param self: Pointer to hcsr04_t struct.
 * @retval esp_err_t
 */
esp_err_t hcsr04_measure( hcsr04_t * self );

#endif
