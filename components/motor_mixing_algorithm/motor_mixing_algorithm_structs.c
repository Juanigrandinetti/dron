#include <motor_mixing_algorithm_structs.h>
#include <pwm.h>

static float saturation( float c_action_p )
{
    float c_action = c_action_p + 5;                    /* No action means 5 % of Duty Cycle for motors, then the 'floor' is 5 */

    if( c_action > MAX_DC )
        return MAX_DC;
    else if( c_action < MIN_DC )
        return MIN_DC;
    else
        return c_action;
}

void mma_update( mma_t * self )
{
    float roll_p  = self->input.c_roll  * (  0.5  );    /* Get half of each Controller output action */
    float roll_n  = self->input.c_roll  * ( -0.5  );

    float pitch_p = self->input.c_pitch * (  0.5  );
    float pitch_n = self->input.c_pitch * ( -0.5  );

    float yaw_p   = self->input.c_yaw   * (  0.5  );
    float yaw_n   = self->input.c_yaw   * ( -0.5  );

    /* Note: z Controller action is distributed equaly to each motor */

    self->output.dc1 = saturation( ( self->input.c_z + pitch_p ) + ( roll_p + yaw_p ) );     /* Compute algorithm */
    self->output.dc2 = saturation( ( self->input.c_z + pitch_p ) + ( roll_n + yaw_n ) );
    self->output.dc3 = saturation( ( self->input.c_z + pitch_n ) + ( roll_n + yaw_p ) );
    self->output.dc4 = saturation( ( self->input.c_z + pitch_n ) + ( roll_p + yaw_n ) );
}
