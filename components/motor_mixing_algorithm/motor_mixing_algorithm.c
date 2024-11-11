#include <stdio.h>
#include <motor_mixing_algorithm.h>

void Mma( mma_t * self )
{
    self->input.c_z     = 0;
    self->input.c_roll  = 0;
    self->input.c_pitch = 0;
    self->input.c_yaw   = 0;
    self->output.dc1    = 0;
    self->output.dc2    = 0;
    self->output.dc3    = 0;
    self->output.dc4    = 0;
    self->update        = mma_update;
}
