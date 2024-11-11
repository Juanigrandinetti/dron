#include <tasks.h>
#include <string.h>



/* ********************************************
                GLOBAL VARIABLES
   ******************************************** */

extern dron_t dron;
TaskHandle_t xHandlerTask;                                      /* Used to handle Bluetooth events tasks */
FILE * fp;                                                      /* Used to access files stored in Spiffs partition */
int cont = 0;                                                   /* Used to totalize the amount of data stored in files */



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void vTaskUpdateDC( void * pvParameters )
{
    while( 1 )
    {
        /* Update pwm Duty Cycle of each motor with Mma object output corresponding attribute */
        dron.components.pwm[ m1 ].set_dc( &( dron.components.pwm[ m1 ] ), dron.components.mma.output.dc1 );
        dron.components.pwm[ m2 ].set_dc( &( dron.components.pwm[ m2 ] ), dron.components.mma.output.dc2 );
        dron.components.pwm[ m3 ].set_dc( &( dron.components.pwm[ m3 ] ), dron.components.mma.output.dc3 );
        dron.components.pwm[ m4 ].set_dc( &( dron.components.pwm[ m4 ] ), dron.components.mma.output.dc4 );

        vTaskDelay( pdMS_TO_TICKS( Ts ) );
    }
}

void vTaskUpdateMMA( void * pvParameters )
{
    while( 1 )
    {
        dron.components.mma.update( &( dron.components.mma ) );     /* Compute Motor Mixing Algorithm and update output signals of Mma object */
        vTaskDelay( pdMS_TO_TICKS( Ts ) );
    }
}

void vTaskPIDz( void * pvParameters )
{
    while( 1 )
    {
        if( z <= N_CONTROL_VARIABLES )
        {
            dron.components.mma.input.c_z = dron.components.controller[ z ].pid(
                &( dron.components.controller[ z ] ),
                dron.states.z,
                dron.sp.z,
                Z_FREQUENCY,
                z,
                anti_windup
            );
        }
        else
            printf( "z index is wrong.\n" );

        vTaskDelay( pdMS_TO_TICKS( Ts ) );
    }
}

void vTaskPIDRoll( void * pvParameters )
{
    while( 1 )
    {
        if( ( roll <= N_CONTROL_VARIABLES ) && ( roll_dot <= N_CONTROL_VARIABLES ) )
        {
            float sp_roll_dot = dron.components.controller[ roll ].pid(
                &( dron.components.controller[ roll ] ),
                dron.states.roll,
                dron.sp.roll,
                ROLL_FREQUENCY,
                roll,
                anti_windup
            );

            dron.components.mma.input.c_roll = dron.components.controller[ roll_dot ].pid(
                &( dron.components.controller[ roll_dot ] ),
                dron.states.roll,
                sp_roll_dot,
                ROLL_DOT_FREQUENCY,
                roll,
                anti_windup
            );
        }
        else
            printf( "roll or roll_dot index is/are wrong.\n" );

        vTaskDelay( pdMS_TO_TICKS( Ts ) );
    }
}


void vTaskPIDPitch( void * pvParameters )
{
    while( 1 )
    {
        if( ( pitch <= N_CONTROL_VARIABLES ) && ( pitch_dot <= N_CONTROL_VARIABLES ) )
        {
            float sp_pitch_dot = dron.components.controller[ pitch ].pid(
                &( dron.components.controller[ pitch ] ),
                dron.states.pitch,
                dron.sp.pitch,
                PITCH_FREQUENCY,
                pitch,
                anti_windup
            );

            dron.components.mma.input.c_pitch = dron.components.controller[ pitch_dot ].pid(
                &( dron.components.controller[ pitch_dot ] ),
                dron.states.pitch,
                sp_pitch_dot,
                PITCH_DOT_FREQUENCY,
                pitch,
                no_saturation
            );
        }
        else
            printf( "pitch or pitch_dot index is/are wrong.\n" );

        vTaskDelay( pdMS_TO_TICKS( Ts ) );
    }
}

void vTaskPIDYaw( void * pvParameters )
{
    while( 1 )
    {
        if( ( yaw <= N_CONTROL_VARIABLES ) & ( yaw_dot <= N_CONTROL_VARIABLES ) )
        {
            float sp_yaw_dot = dron.components.controller[ yaw ].pid(
                &( dron.components.controller[ yaw ] ),
                dron.states.yaw,
                dron.sp.yaw,
                YAW_FREQUENCY,
                yaw,
                anti_windup
            );

            dron.components.mma.input.c_yaw = dron.components.controller[ yaw_dot ].pid(
                &( dron.components.controller[ yaw_dot ] ),
                dron.states.yaw,
                sp_yaw_dot,
                YAW_DOT_FREQUENCY,
                yaw,
                no_saturation
            );
        }
        else
            printf( "yaw or yaw_dot index is/are wrong.\n" );

        vTaskDelay( pdMS_TO_TICKS( Ts ) );
    }
}

void vTaskPrintStates( void * pvParameters )
{
    while ( 1 )
    {
        printf( "z:      %.2f    [ cm ]\t|\tSP z:   %.2f\n", dron.states.z    [ 1 ], dron.sp.z );
        printf( "Roll:  %.2f    [ ° ]\t|\tSP Roll:  %.2f\n", dron.states.roll [ 1 ], dron.sp.roll );
        printf( "Pitch: %.2f    [ ° ]\t|\tSP Pitch: %.2f\n", dron.states.pitch[ 1 ], dron.sp.pitch );
        printf( "Yaw:   %.2f    [ ° ]\t|\tSP Yaw:   %.2f\n", dron.states.yaw  [ 1 ], dron.sp.yaw );

        printf( "DC M1: %.2f\n", dron.components.pwm[ m1 ].get_dc( &( dron.components.pwm[ m1 ] ) ) );
        printf( "DC M2: %.2f\n", dron.components.pwm[ m2 ].get_dc( &( dron.components.pwm[ m2 ] ) ) );
        printf( "DC M3: %.2f\n", dron.components.pwm[ m3 ].get_dc( &( dron.components.pwm[ m3 ] ) ) );
        printf( "DC M4: %.2f\n", dron.components.pwm[ m4 ].get_dc( &( dron.components.pwm[ m4 ] ) ) );
        
        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
    }
}

void vTaskMeasureDistance( void * pvParameters )
{
    while ( 1 )
    {
        dron.measure.distance( &dron );

        vTaskDelay( pdMS_TO_TICKS( Ts ) );
    }
}

void vTaskUpdateDistance( void * pvParameters )
{
    while ( 1 )
    {
        dron.update.distance( &dron );

        vTaskDelay( pdMS_TO_TICKS( Ts ) );
    }
}

void vTaskUpdateAngles( void * pvParameters )
{
    while( 1 )
    {       
        dron.update.angles( &dron );

        vTaskDelay( pdMS_TO_TICKS( 10 ) );
    }
}

void vTaskMeasureAngles( void * pvParameters )
{
    while( 1 )
    {
        dron.measure.angles( &dron );
        
        vTaskDelay( pdMS_TO_TICKS( 10 ) );
    }
}

void vTaskHandleBtFlags( void * pvParameters )
{
    bt_flags_t * flag = ( bt_flags_t * ) pvParameters;
    while( 1 )
    {
        switch( flag->new_dev )
        {
        case 1:
            gpio_set_level( dron.components.bt.new_device_gpio, 1 );
            break;
        case 0:
            gpio_set_level( dron.components.bt.new_device_gpio, 0 );
            break;
        default:
            printf( "Flag New device wrong value!\n" );
            break;
        }

        switch( flag->data_received )
        {
        case 1:
            parse_bt_cmd( flag->data );
            flag->data_received = 0;
            break;
        case 0:
            break;
        default:
            printf( "Flag Data received wrong value!\n" );
            break;
        }
        vTaskDelay( pdMS_TO_TICKS( 250 ) );
    }
}

static void parse_bt_cmd( char * data )
{
    eTaskState task_state;                          /* Used to get state of store bytes FreeRTOS Task */
    char* token;
    char* rest = data;
                                                    /**
                                                     * @brief Bluetooth commands tokens[ 4 ] = { type, state, pid type, new value }.
                                                     * @param type: Type of command ( 'pid', 'start', 'stop', 'send', 'erase' ).
                                                     * @param state: State ( z, roll, pitch, yaw. Only applicable for 'pid' command ).
                                                     * @param pid_type: PID type of action ( p, i, d. Only applicable for 'pid' command ).
                                                     * @param value: New pid value ( Only applicable for 'pid' commando ).
                                                     * @example: <pid,z,p,10>
                                                     */
    char* tokens[ 4 ] = { "", "", "", "" };
    int i = 0;
    while( ( token = strtok_r( rest, ",", &rest ) ) )       /* Process command separately */
    {
        tokens[ i ] = token;
        i++;
    }

    if( strcmp( tokens[ 0 ], "pid" ) == 0 )                 /* Check if it's a 'pid' command */
    {
        dron.components.bt.pid_type = tokens[ 2 ];
        dron.components.bt.value    = tokens[ 3 ];

        if( strcmp( tokens[ 1 ], "z" )          == 0 )      /* Check which state variable corresponds */
            dron.components.bt.state = z;
        else if( strcmp( tokens[ 0 ], "roll" )  == 0 )
            dron.components.bt.state = roll;
        else if( strcmp( tokens[ 0 ], "pitch" ) == 0 )
            dron.components.bt.state = pitch;
        else if( strcmp( tokens[ 0 ], "yaw" )   == 0 )
            dron.components.bt.state = yaw;
        else
            dron.components.bt.state = -1;
        xTaskCreate( vTaskUpdatePIDParameters, "vTaskUpdatePIDParameters", BT_TASK_STACK * 2, NULL, 1, NULL );
    }
    else if( strcmp( tokens[ 0 ], "start" ) == 0 )                                                              /* Check if it's a 'start' command */
    {
        printf( "start cmd\n" );
        gpio_set_level( dron.components.bt.load_gpio, 1 );                                                      /* Turn Green LED ON */
        if( xHandlerTask == NULL )                                                                              /* Check if Task doesn't exist */
            xTaskCreate( vTaskLoadToFlash, "vTaskLoadToFlash", BT_TASK_STACK * 2, NULL, 1, &xHandlerTask );     /* If it doesn't, then create it */
        else
        {
            vTaskResume( xHandlerTask );                                                                        /* If it exists, then resume it */
            printf( "Load bytes to Flash memory resumed.\n" );
        }
    }
    else if( strcmp( tokens[ 0 ], "stop" ) == 0 )
    {
        if( xHandlerTask != NULL )                                              /* Check if Task doesn't exist */
        {
            task_state = eTaskGetState( xHandlerTask );                         /* Get Task state */
            while ( !task_state );                                              /* Wait until load bytes to Flash memory task has finished */
            gpio_set_level( dron.components.bt.load_gpio, 0 );                  /* Turn Green LED OFF */
            vTaskSuspend( xHandlerTask );                                       /* Suspend loading bytes to Flash memory task */
            printf( "Load bytes to Flash memory has been suspended.\n" );
        }
        else
            printf( "File is empty.\n" );
    }
    else if( strcmp( tokens[ 0 ], "send" ) == 0 )
    {
        if( xHandlerTask != NULL )                              /* Check if Task doesn't exist */
        {
            task_state = eTaskGetState( xHandlerTask );         /* Get Task state */
            while( !task_state );                               /* Wait until load bytes to Flash memory task has finished */
            gpio_set_level( dron.components.bt.load_gpio, 0 );  /* Turn Green LED OFF */
            vTaskSuspend( xHandlerTask );                       /* Suspend loading bytes to Flash memory task */
            xTaskCreate( vTaskExctractFromFlash, "vTaskExctractFromFlash", BT_TASK_STACK * 2, NULL, 1, NULL );  /* Create FreeRTOS Task to extract bytes from Flash memory */
        }
        else
            printf( "Files does not exist.\n" );
    }
    else if( strcmp( tokens[ 0 ], "erase" ) == 0 )
    {
        if( xHandlerTask != NULL )                              /* Check if Task doesn't exist */
        {
            task_state = eTaskGetState( xHandlerTask );         /* Get Task state */
            while( !task_state );                               /* Wait until load bytes to Flash memory task has finished */
            gpio_set_level( dron.components.bt.load_gpio, 0 );  /* Turn Green LED OFF */
            vTaskDelete( xHandlerTask );                        /* Delete loading bytes to Flash memory task */
            xHandlerTask = NULL;                                /* Initialize loading bytes to Flash memory Handler to NULL again */
            xTaskCreate( vTaskEraseFlash, "vTaskEraseFlash", BT_TASK_STACK * 2, NULL, 1, NULL );    /* Create FreeRTOS Task to erase bytes from Flash memory */
        }
        else
            printf( "File does not exist.\n" );
    }
}


static void vTaskExctractFromFlash( void * pvParameters )
{
    char stored_data[ 256 ];                                                /* Create Buffer to store data extracted from Flash memory */

    while( 1 )
    {
        fp = fopen( dron.components.spiffs.filename[ 0 ], "r" );            /* Open file in Read mode */
        if( fp != NULL )                                                    /* Check if the file was accessed correctly */
        {
            while( fgets( stored_data, sizeof( stored_data ), fp ) )        /* Get data */
                printf( "%s", stored_data );
            fclose( fp );                                                   /* Close file */
            vTaskDelete( NULL );                                            /* Delete actual Task */
        }
        else                                                                /* Opening file error */
            printf( "Failed reading bytes from file.\n" );
        fclose( fp );                                                       /* Close file */
        vTaskDelete( NULL );                                                /* Delete actual Task */
    }
}


static void vTaskLoadToFlash( void * pvParameters )
{
    while( 1 )
    {
        dron.components.spiffs.info( &( dron.components.spiffs ) );         /* Print Spiffs partition information */
        fp = fopen( dron.components.spiffs.filename[ 0 ], "a" );            /* Open file in Append mode */
        if( fp != NULL )                                                    /* Check if the file was accessed correctly */
        {
            fprintf( fp, "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",       /* Print SetPoints and States  */
                dron.sp.z,     dron.states.z    [ 1 ],                      /* SetPoint: z,     State: z */
                dron.sp.roll,  dron.states.roll [ 1 ],                      /* SetPoint: Roll,  State: Roll */    
                dron.sp.pitch, dron.states.pitch[ 1 ],                      /* SetPoint: Pitch, State: Pitch */    
                dron.sp.yaw,   dron.states.yaw  [ 1 ]                       /* SetPoint: Yaw,   State: Yaw */
            );
            printf( "Stored values: %d\n", cont + 1 );
            cont++;                                                         /* Update total data counter */
        }
        else                                                                /* Opening file error */
        {
            printf( "Failed writing bytes to file.\n" );
            fclose( fp );                                                   /* Close file */
            vTaskDelete( NULL );                                            /* Delete actual Task */
        }
        fclose( fp );                                                       /* Close file */
        vTaskDelay( pdMS_TO_TICKS( 500 ) );
    }
}


static void vTaskUpdatePIDParameters( void * pvParameters )
{
    while( 1 )
    {
        if( strcmp( dron.components.bt.pid_type, "p" ) == 0 )                                                                /* Check if it's a Proportional update */
        {
            dron.components.controller[ dron.components.bt.state ].gain.kp = atof( dron.components.bt.value );
            switch( dron.components.bt.state )
            {
                case 0:
                    printf( "{ z } Kp: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.kp );            /* State update: z */
                    break;
                case 1:
                    printf( "{ Roll } Kp: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.kp );         /* State update: Roll */
                    break;
                case 2:
                    printf( "{ Pitch } Kp: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.kp );        /* State update: Pitch */
                    break;
                case 3:
                    printf( "{ Yaw } Kp: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.kp );          /* State update: Yaw */
                    break;
                default:
                    break;
            }
        }
        else
        {
            if( strcmp( dron.components.bt.pid_type, "i" ) == 0 )                                                            /* Check if it's a Integral update */
            {
                dron.components.controller[ dron.components.bt.state ].gain.ki = atof( dron.components.bt.value );
                switch( dron.components.bt.state )
                {
                    case 0:
                        printf( "{ z } Ki: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.ki );        /* State update: z */
                        break;
                    case 1:
                        printf( "{ Roll } Ki: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.ki );     /* State update: Roll */
                        break;
                    case 2:
                        printf( "{ Pitch } Ki: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.ki );    /* State update: Pitch */ 
                        break;
                    case 3:
                        printf( "{ Yaw } Ki: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.ki );      /* State update: Yaw */
                        break;
                    default:
                        break;
                }
            }
            else
            {
                if( strcmp( dron.components.bt.pid_type, "d" ) == 0 )                                                           /* Check if it's a Derivative update */
                {
                    dron.components.controller[ dron.components.bt.state ].gain.kd = atof( dron.components.bt.value );
                    switch( dron.components.bt.state )
                    {
                        case 0:
                            printf( "{ z } Kd: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.kd );       /* State update: z */ 
                            break;
                        case 1:
                            printf( "{ Roll } Kd: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.kd );    /* State update: Roll */
                            break;
                        case 2:
                            printf( "{ Pitch } Kd: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.kd );   /* State update: Pitch */
                            break;
                        case 3:
                            printf( "{ Yaw } Kd: %.2f\n", dron.components.controller[ dron.components.bt.state ].gain.kd );     /* State update: Yaw */
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        vTaskDelete( NULL );
    }
}


static void vTaskEraseFlash( void * pvParameters )
{
    gpio_set_level( dron.components.bt.erase_gpio, 1 );                     /* Turn Red LED ON */
    while( 1 )
    {
        if( remove( dron.components.spiffs.filename[ 0 ] ) == 0 )           /* Remove file from spiffs partition */
        {
            printf( "File has been removed successfully.\n" );
            cont = 0;                                                       /* Re-initialize total data counter */
        }
        else                                                                /* Removing file error */
            printf( "Failed to remove file.\n" );
        vTaskDelay( pdMS_TO_TICKS( 1000 ) );
        gpio_set_level( dron.components.bt.erase_gpio, 0 );                 /* Turn Red LED OFF */
        vTaskDelete( NULL );                                                /* Delete actual Task */
    }
}
