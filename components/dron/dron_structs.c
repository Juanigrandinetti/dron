#include <dron_structs.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void update_distance( dron_t * self )
{
    /* time [ ms ] */
    /* velocity [ cm/ms ] */
    /* distance [ cm ] */

    if( self->components.hcsr04.time != -1 )                                                /* Check if distance was succesfully measured */
    {
        self->states.z[ 0 ] = self->states.z[ 1 ];
        self->states.z[ 1 ] = ( self->components.hcsr04.time * 1.0) * ( V_SONIDO / 2 );          /* distance = ( time / 2 ) * velocity */
    }
    else
        printf( "Distance out of range.\n" );

    /* Note: 2 in equation is beacuse ultrasonic wave goes and return back to Rx sensor, so it travels the distance between the sensor and the object, twice */
}

void update_angles( dron_t * self )
{
    float pitch_acc = atan2( -self->components.mpu.Acc.x,
                            sqrt( ( self->components.mpu.Acc.y * self->components.mpu.Acc.y ) + ( self->components.mpu.Acc.z * self->components.mpu.Acc.z ) )
                        ) * ( 180.0 / M_PI );
    
    float roll_acc = atan2( self->components.mpu.Acc.y, self->components.mpu.Acc.z ) * ( 180.0 / M_PI );
    float pitch_gyro = self->states.pitch[ 1 ] + self->components.mpu.Gyro.y * 0.05;
    float roll_gyro  = self->states.roll [ 1 ] + self->components.mpu.Gyro.x * 0.05;

    self->states.yaw  [ 0 ] = self->states.yaw  [ 1 ];
    self->states.pitch[ 0 ] = self->states.pitch[ 1 ];
    self->states.roll [ 0 ] = self->states.roll [ 1 ];

    self->states.yaw  [ 1 ] += self->components.mpu.Gyro.z * 0.05;
    self->states.pitch[ 1 ]  = 0.98 * roll_acc + 0.02 * roll_gyro;          /* Change state.'roll' for state.'pitch' because mpu axes don't match dron's axes */
    self->states.roll [ 1 ]  = 0.98 * pitch_acc  + 0.02 * pitch_gyro;       /* Change state.'pitch' for state.'roll' because mpu axes don't match dron's axes */
}

void measure_distance( dron_t * self )
{
    self->components.hcsr04.measure( &( self->components.hcsr04 ) );
}

void measure_angles( dron_t * self )
{
    self->components.mpu.measure( &( self->components.mpu ) );
}
