#include <pwm_structs.h>

/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void increase_dc( pwm_t * self )
{
    if ( mcpwm_get_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen ) < self->cfg.max_dc )
        mcpwm_set_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen, mcpwm_get_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen ) + 0.01 );
    printf( "DC: %.2f\n", mcpwm_get_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen ) );
}

void decrease_dc( pwm_t * self )
{
    if ( mcpwm_get_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen ) > self->cfg.min_dc )
        mcpwm_set_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen, mcpwm_get_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen ) - 0.01 );
    printf( "DC: %.2f\n", mcpwm_get_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen ) );
}

void set_dc( pwm_t * self, float dc )
{
    mcpwm_set_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen, dc );       /* Set Duty Cycle */
}

float get_dc( pwm_t * self )
{
    return mcpwm_get_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen );
}
