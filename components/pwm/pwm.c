#include <stdio.h>
#include <pwm.h>

/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

esp_err_t Pwm( pwm_t * self, pwm_cfg_t * pwm_cfg_p )
{
    self->cfg.gpio                = pwm_cfg_p->gpio;                        /* Assign given parameters to object attributes */
    self->cfg.max_dc              = pwm_cfg_p->max_dc;
    self->cfg.min_dc              = pwm_cfg_p->min_dc;
    self->cfg.unit                = pwm_cfg_p->unit;
    self->cfg.timer               = pwm_cfg_p->timer;
    self->cfg.gen                 = pwm_cfg_p->gen;
    self->cfg.mcpwm_cfg.frequency = pwm_cfg_p->mcpwm_cfg.frequency;
    if( pwm_cfg_p->gen == MCPWM_GEN_A )                                     /* If using Generator A, set Duty Cycle % to operator A */
        self->cfg.mcpwm_cfg.cmpr_a = pwm_cfg_p->mcpwm_cfg.cmpr_a;
    else                                                                    /* If using Generator B, set Duty Cycle % to operator B */
        self->cfg.mcpwm_cfg.cmpr_b = pwm_cfg_p->mcpwm_cfg.cmpr_b;

    self->cfg.mcpwm_cfg.duty_mode    = pwm_cfg_p->mcpwm_cfg.duty_mode;
    self->cfg.mcpwm_cfg.counter_mode = pwm_cfg_p->mcpwm_cfg.counter_mode;
    self->increase_dc      = increase_dc;                                   /* Pointer assignment to increase_dc function */
    self->decrease_dc      = decrease_dc;                                   /* Pointer assignment to decrease_dc function */
    self->set_dc           = set_dc;                                        /* Pointer assignment to set_dc function */
    self->get_dc           = get_dc;                                        /* Pointer assignment to get_dc function */

    esp_err_t res = pwm_init( self );                                       /* Initialize Pwm object */

    return res;
}

esp_err_t pwm_init( pwm_t * self )
{
    if( mcpwm_gpio_init( self->cfg.unit, self->cfg.gen, self->cfg.gpio ) != ESP_OK )            /* Initialize pwm GPIO */
        return ESP_FAIL;

    if( mcpwm_init( self->cfg.unit, self->cfg.timer, &( self->cfg.mcpwm_cfg ) ) != ESP_OK )     /* Initialize pwm Unit */
        return ESP_FAIL;
    
    mcpwm_set_duty( self->cfg.unit, self->cfg.timer, self->cfg.gen, self->cfg.min_dc );         /* Initialize Duty Cycle with minimum value */

    printf( "PWM initialized.\n" );

    return ESP_OK;
}
