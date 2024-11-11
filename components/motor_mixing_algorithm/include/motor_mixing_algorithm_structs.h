#ifndef STRUCT_MOTOR_MIXING_ALGORITHM
#define STRUCT_MOTOR_MIXING_ALGORITHM



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <quadcopter_controller.h>



/* *****************************************
                OBJECT STRUCT
   ***************************************** */

/**
 * @brief motor_crtl_t struct used to store Motor Mixing Algorithm output action ( this signal controlls motor speed ).
 * @param dc1: Duty Cycle of Motor 1.
 * @param dc2: Duty Cycle of Motor 2.
 * @param dc3: Duty Cycle of Motor 3.
 * @param dc4: Duty Cycle of Motor 4.
 */
typedef struct motor_crtl
{
    float dc1;
    float dc2;
    float dc3;
    float dc4;
}motor_crtl_t;

/**
 * @brief controller_action_t struct used to store Controllers output action.
 * @param c_z: z Controller output.
 * @param c_roll: Roll Controller output.
 * @param c_pitch: Pitch Controller output.
 * @param c_yaw: Yaw Controller output.
 */
typedef struct controller_action
{
    float c_z;
    float c_roll;
    float c_pitch;
    float c_yaw;
}controller_action_t;

/**
 * @brief mma_t struct used to controll motors speed.
 * @param input: Controllers output.
 * @param output: Signal that controlls pwm of each motor.
 * @param update: Update controll signals.
 */
typedef struct mma
{
    /**
     * Attributes
     * ---------
     * 
     */
    controller_action_t input;
    motor_crtl_t output;

    /**
     * Methods
     * -------
     */

    /**
     * @brief Update all output signals of Mma object, used to control pwm of each motor.
     * @param self: Pointer to mma_t struct.
     * @retval none
     */
    void ( *update )( struct mma_t * self );
}mma_t;



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief Check if the motor speed controll signal is greater ( lower ) than maximum ( minimum ).
 * @param c_action: Controller output action.
 * @retval float
 */
static float saturation( float c_action_p );

/**
 * @brief Update all output signals of Mma object, used to control pwm of each motor.
 * @param self: Pointer to mma_t struct.
 * @retval none
 */
void mma_update( mma_t * self );

#endif
