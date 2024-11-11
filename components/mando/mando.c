#include <stdio.h>
#include <mando.h>
#include <dron.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

esp_err_t Joystick( joystick_t * self, const uint8_t mac_p[ MAC_ADDR_SIZE ] )
{
    for (size_t i = 0; i < MAC_ADDR_SIZE; i++)                      /* Assign parameter to object attribute */
        self->mac[ i ] = mac_p[ i ];
    
    esp_err_t res = joystick_init( self );                          /* Initialize Joystick */

    return res;
}

void controller_event_cb( ps3_t ps3, ps3_event_t event )
{
    extern dron_t dron;
    if( ps3.button.up )                                                 /* z state ( + ) */
    {
        if( dron.sp.z + SP_DELTA_Z < MAX_Z_CM )
            dron.sp.z = dron.sp.z + SP_DELTA_Z;                         /* Update z SetPoint */
    }
    else if( ps3.button.down )                                          /* z state ( - ) */
    {
        if( dron.sp.z - SP_DELTA_Z > 0 )
            dron.sp.z = dron.sp.z - SP_DELTA_Z;                         /* Update z SetPoint */
    }
    else if( ps3.button.triangle )                                      /* Pitch state ( + ) */
    {
        if( dron.sp.pitch + SP_DELTA_PITCH < MAX_ANGLE )
            dron.sp.pitch = dron.sp.pitch + SP_DELTA_PITCH;             /* Update Pitch SetPoint */
    }
    else if( ps3.button.cross )                                         /* Pitch state ( - ) */
    {
        if( dron.sp.pitch - SP_DELTA_PITCH > ( - MAX_ANGLE ) )
            dron.sp.pitch= dron.sp.pitch - SP_DELTA_PITCH;              /* Update Pitch SetPoint */
    }
    else if( ps3.button.circle )                                        /* Roll state ( + ) */
    {
        if( dron.sp.roll + SP_DELTA_ROLL < MAX_ANGLE )
            dron.sp.roll = dron.sp.roll + SP_DELTA_ROLL;                /* Update Roll SetPoint */
    }
    else if( ps3.button.square )                                        /* Roll state ( - ) */
    {
        if( dron.sp.roll - SP_DELTA_ROLL > ( - MAX_ANGLE ) )
            dron.sp.roll = dron.sp. roll- SP_DELTA_ROLL;                /* Update z SetPoint */
    }
    else if( ps3.button.right )                                         /* Yaw state ( + ) */
    {
        if( dron.sp.yaw + SP_DELTA_YAW < MAX_ANGLE )
            dron.sp.yaw = dron.sp.yaw + SP_DELTA_YAW;                   /* Update Yaw SetPoint */
    }
    else if( ps3.button.left )                                          /* Yaw state ( - ) */
    {
        if( dron.sp.yaw - SP_DELTA_YAW > ( - MAX_ANGLE ) )
            dron.sp.yaw = dron.sp.yaw - SP_DELTA_YAW;                   /* Update Yaw SetPoint */
    }
}

esp_err_t joystick_init( joystick_t * self )
{
    mac_address_t mac_obj;
    Mac( &mac_obj, &( self->mac ) );                                                    /* Make an instance of Mac class */
    mac_obj.get_mac_address( &mac_obj );
    esp_err_t res = mac_obj.set_mac_address( &mac_obj, &( self->mac ) );                /* Set Joystick's MAC Address */
    if( res != ESP_OK )
        return ESP_FAIL;
    ps3SetEventCallback( controller_event_cb );                                         /* Enable Joystick Bluetooth Callback */
    ps3SetBluetoothMacAddress( mac_obj.mac );
    ps3Init();                                                                          /* Initialize Ps3 */
    gpio_reset_pin( 2 );                                                                /* Turn esp'led ( GPIO2 ) on to inform Joystick has been initialized */
    gpio_set_direction( 2, GPIO_MODE_OUTPUT );
    gpio_set_level( 2, 1 );
    while( !ps3IsConnected() )                                                          /* Wait until Joystick is connected */
        vTaskDelay( pdMS_TO_TICKS( 10 ) );

    printf( "Flight controller initialized.\n" );

    return ESP_OK;
}
