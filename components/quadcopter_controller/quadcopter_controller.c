#include <stdio.h>
#include <quadcopter_controller.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void Pid( pid_controller_t * self, float kp_p, float ki_p, float kd_p )
{
    /**
     * Attributes
     * ----------
     * 
     */
    self->gain.kp            = kp_p;
    self->gain.ki            = ki_p;
    self->gain.kd            = kd_p;
    self->action.p           = 0;
    for (int i = 0; i < INTEGRAL_BUFFER_SIZE; i++)
        self->action.i_buffer[ i ] = 0;
    for (int i = 0; i < DERIVATE_BUFFER_SIZE; i++)
        self->action.d_buffer[ i ] = 0;
    for (int i = 0; i < ERROR_BUFFER_SIZE; i++)
        self->error_buffer[ i ]    = 0;

    /**
     * Methods
     * -------
     * 
     */
    self->pid = pid;
}
