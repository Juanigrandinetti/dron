#include <quadcopter_controller_structs.h>
#include <esp_clk_tree.h>
#include <string.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

float pid( pid_controller_t * self, float state_x[ 2 ], float sp_x, float fc, states_e state_label, int_sat_e integral_sat )
{
    float N_x = 2 * M_PI * fc;

    self->error_buffer[ 0 ] = self->error_buffer[ 1 ];          /* Update last Error */
    self->error_buffer[ 1 ] = sp_x - state_x[ 1 ];              /* Update new Error */

    /* Note: Roll, Pitch and Yaw SetPoints and measures are in angles but the model of the drone is in radians, so we need to convert them to radians */
    if(  state_label != z )
        self->error_buffer[ 1 ] *= ( M_PI / 180 );

    self->action.p = self->error_buffer[ 1 ];                   /* Update Proportional value */


    /**
     * *********************************************************************************
     * 
     *         Controller Discretization with Bilinear ( Tustin ) Approximation
     * 
     * *********************************************************************************
     */

    /** Integral Discretization:
     * 
     *                         Ts
     * y[ n ] = y[ n - 1 ]  + ---- * ( x[ n ] + x[ n - 1 ] ) = a + ( b * c ) where,
     *                         2
     * x is the error and
     * y is the integral value
     */

    self->action.i_buffer[ 0 ] = self->action.i_buffer[ 1 ];            /* Update last Integral value */
    float a = self->action.i_buffer[ 0 ];
    float b = Ts / 2;
    float c = self->error_buffer[ 1 ] + self->error_buffer[ 0 ];

    self->action.i_buffer[ 1 ] = a + ( b * c );                         /* Update new Integral value */
    a = b = c = 0;


    /**
     * Derivative Discretization:
     * 
     *            [ 2 * N * ( x[ n ] - x[ n - 1 ] ) ] + [ ( 2 - ( N * Ts ) ) * y[ n - 1 ] ]      a + b
     * y[ n ] = ----------------------------------------------------------------------------- = ------- where,
     *                                        2 + ( N * Ts )                                       c
     * 
     * x is the state ( in PI-D control, the input to D action is the state not the error ) and
     * y is the derivative value
     */

    self->action.d_buffer[ 0 ] = self->action.d_buffer[ 1 ];                    /* Update last Derivative value */
    a = 2 * N_x * ( state_x[ 1 ] - state_x[ 0 ] );
    b = ( 2 - ( N_x * Ts ) ) * self->action.d_buffer[ 0 ];
    c = 2 + ( N_x * Ts );

    self->action.d_buffer[ 1 ] = ( a + b ) / c;                                 /* Update new Derivative value */
    a = b = c = 0;

    float c_p = 0;
    float c_i = 0;
    float c_d = 0;

    /**
     * ***********************************
     * 
     *         Proportional Action
     * 
     * ***********************************
     */
    
    /*if( ( state_label == z ) && ( state_x[ 1 ] < Z_MIN ) )
        c_p = self->action.p * self->gain.kp_take_off;
    else
        c_p = self->action.p * self->gain.kp;*/
    c_p = self->action.p * self->gain.kp;

    /**
     * ***********************************
     * 
     *         Integral Action
     * 
     * ***********************************
     */
    
    if( integral_sat == anti_windup )                                                       /* Anti-Windup Algorithm */
    {
        if( ( self->error_buffer[ 1 ] < 0 ) && ( self->action.i_buffer[ 1 ] > 0 ) )     /* Check anti-windup condition */
            c_i = 0;                        /* Integral Action is saturated, then shut it down */
        else
            c_i = self->action.i_buffer[ 1 ] * self->gain.ki;       /* Integral Action is not saturated, then perform normal calculation */
    }
    else if( integral_sat == back_propagation )                                             /* Back-Propagation Algotihm */
    {
        /* Back-Propagation Algorithm */
    }
    else
        c_i = self->action.i_buffer[ 1 ] * self->gain.ki;       /* Perform calculation without integral saturation */


    /**
     * ***********************************
     * 
     *         Derivative Action
     * 
     * ***********************************
     */

    c_d = self->action.d_buffer[ 1 ] * self->gain.kd;

    /**
     * In Classic PID Control, the Controller Action is calculated by C = P + I + D.
     * However, we are implementing a PI-D Controller, hence, the controller action is
     * calculated by C = P + I - D.
     * 
     * For more information, see "tesis" and Simulink models.
     */
    

    return  c_p + c_i - c_d ;       /* Return Controller Action */
}
