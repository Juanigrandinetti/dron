#ifndef QUADCOPTER
#define QUADCOPTER



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <dron_structs.h>



/* ********************************************
                    DEFINES
   ******************************************** */

#define N_MEASURES            40                   /* Total samples for Roll, Pitch and z used to caibrate their respective SetPoints */



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief Calibrate Roll, Pitch and z SetPoints according to MPU and HCSR-04 static measures.
 * @param self: Pointer to dron_t struct.
 * @retval none
 */
void dron_calibration( dron_t * self );

/**
 * @brief Make an instance of Dron class.
 * @param self: Pointer to dron_t struct.
 * @param dron_cfg_p: Pointer to dron_cfg_t struct.
 * @retval esp_err_t
 */
esp_err_t Dron( dron_t * self, dron_cfg_t * dron_cfg_p );

#endif
