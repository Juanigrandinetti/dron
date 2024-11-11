#ifndef STRUCT_QUADCOPTER_CONTROLLER
#define STRUCT_QUADCOPTER_CONTROLLER



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <math.h>



/* ********************************************
                    DEFINES
   ******************************************** */

#define DERIVATE_BUFFER_SIZE          2                 /* Total elements of Derivative Buffer */
#define INTEGRAL_BUFFER_SIZE          2                 /* Total elements of Integral Buffer */
#define ERROR_BUFFER_SIZE             2                 /* Total elements of Error Buffer */
#define Ts                            20                /* Samplilng Time of Controllers in ms */
#define Z_MIN                         1                 /* Distance between dron and floor */



/* *****************************************
                OBJECT STRUCT
   ***************************************** */

typedef enum states_e
{
    z,              /* z              index used in Arrays */
    roll,           /* Roll           index used in Arrays */
    pitch,          /* Pitch          index used in Arrays */
    yaw,            /* Yaw            index used in Arrays */
    roll_dot,       /* Roll  Derivate index used in Arrays */
    pitch_dot,      /* Pitch Derivate index used in Arrays */
    yaw_dot,        /* Yaw   Derivate index used in Arrays */    
}states_e;

typedef enum int_sat
{
    no_saturation,
    anti_windup,
    back_propagation,
}int_sat_e;

/**
 * @brief pid_gain_t struct with PID types of gain.
 * @param kp: Proportional gain.
 * @param ki: Integral gain.
 * @param kd: Derivative gain.
 */
typedef struct pid_gain
{
    float kp;               /* Proportional gain */
    float kp_take_off;      /* Proportional gain used when taking off */
    float ki;               /* Integral gain */
    float kd;               /* Derivative gain */
}pid_gain_t;

/**
 * @brief pid_action_t struct with PID types of action.
 * @param p: Proportional action.
 * @param i_buffer: Integral action buffer [ Ci_n-1, Ci_n ].
 * @param d_buffer: Derivative action buffer [ Cd_n-1, Cd_n ].
 */
typedef struct pid_action
{
    float p;                                        /* Proportional action. */
    float i_buffer[ INTEGRAL_BUFFER_SIZE ];         /* Integral action buffer [ Ci_n-1, Ci_n ] */
    float d_buffer[ DERIVATE_BUFFER_SIZE ];         /* Derivative action buffer [ Cd_n-1, Cd_n ] */
}pid_action_t;

/**
 * @brief pid_t struct.
 * @param sp: Set Point.
 * @param gain: PID gain struct.
 * @param action: PID action struct
 * @param integal_sum: Sum of the integral of the error.
 * @param update: Update controller errors.
 * @param get_pid: Update controller action.
 */
typedef struct pid_controller
{
    /**
     * Attributes
     * ----------
     * 
     */
    float error_buffer[ ERROR_BUFFER_SIZE ];            /* Error buffer [ e_n-1, e_n ] */
    pid_gain_t gain;                                    /* Controller Gains struct */
    pid_action_t action;                                /* Controller Actions struct */
    /**
     * Methods
     * -------
     * 
     */

    /**
     * @brief Calculate Controller action.
     * @param self: Pointer to pid_t struct.
     * @param state: State variable ( z, Roll, Pitch or Yaw ).
     * @param filter_coefficient: Filter coefficient used in Derivative action.
     * @param state_label: Name of the state variable.
     * @retval none
     */
    float ( *pid )( struct pid_controller_t * self, float state_x[ 2 ], float sp_x, float fc, states_e state_label, int_sat_e integral_sat );
}pid_controller_t;



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief Calculate Controller action.
 * @param self: Pointer to pid_t struct.
 * @param state: State variable ( z, Roll, Pitch or Yaw ).
 * @param filter_coefficient: Filter coefficient used in Derivative action.
 * @param state_label: Name of the state variable.
 * @retval none
 */
float pid( pid_controller_t * self, float state_x[ 2 ], float sp_x, float fc, states_e state_label, int_sat_e integral_sat );

#endif
