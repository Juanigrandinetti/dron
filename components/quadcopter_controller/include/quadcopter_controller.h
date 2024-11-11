#ifndef QUADCOPTER_CONTROLLER
#define QUADCOPTER_CONTROLLER



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <quadcopter_controller_structs.h>



/* ********************************************
                    DEFINES
   ******************************************** */

#define SP_DELTA_Z                       0.01               /* Delta z in meters */
#define SP_DELTA_ROLL                    0.1                /* Delta Roll in angles */
#define SP_DELTA_PITCH                   0.1                /* Delta Pitch in angles */
#define SP_DELTA_YAW                     0.1                /* Delta Yaw in angles */
#define MAX_Z_CM                         100                /* Maximum SetPoint for z state */
#define MAX_ANGLE                        5                  /* Maximum angle rotation for Roll, Pitch and Yaw states */



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief Make an instance of Pid class.
 * @param self: Pointer to pid_t struct.
 * @param kp: Proportional gain.
 * @param ki: Integral gain.
 * @param kd: Derivative gain.
 * @retval none
 */
void Pid( pid_controller_t * self, float kp_p, float ki_p, float kd_p );

#endif
