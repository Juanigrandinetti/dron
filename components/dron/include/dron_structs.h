#ifndef STRUCT_QUADCOPTER
#define STRUCT_QUADCOPTER



/* ********************************************
                    INCLUDES 
   ******************************************** */

#include <mpu9250.h>
#include <spiffs_partition.h>
#include <quadcopter_controller.h>
#include <bt_events.h>
#include <pwm.h>
#include <hcsr04.h>
#include <mando.h>
#include <motor_mixing_algorithm.h>



/* ********************************************
                    DEFINES
   ******************************************** */

#define MAX_DC                      7                               /* Maximum Duty Cycle ( maximum motor speed ). */
#define MIN_DC                      5                               /* Mínimo Duty Cylce ( motor shut down ). */
#define FREQUENCY                   50                              /* Motor working frequency */
#define HCSR04_TRIG_GPIO            27                              /* Trigger GPIO */
#define HCSR04_ECHO_GPIO            14                              /* Echo GPIO */
#define PWM_M1_GPIO                 26                              /* Motor 1 GPIO */
#define PWM_M2_GPIO                 5                               /* Motor 2 GPIO */
#define PWM_M3_GPIO                 15                              /* Motor 3 GPIO */
#define PWM_M4_GPIO                 18                              /* Motor 4 GPIO */
#define Kp_z                        20000                           /* Proportional z gain */
#define Kp_take_off_z               1000000                         /* Proportional z Take-Off gain */
#define Ki_z                        35000                           /* Integral z gain */
#define Kd_z                        3000                            /* Derivative z gain */
#define Kp_roll                     780                             /* Proportional roll gain */
#define Ki_roll                     0.0295                          /* Integral roll gain */
#define Kd_roll                     9.325                           /* Derivative roll gain */
#define Kp_pitch                    1000                            /* Proportional pitch gain */
#define Ki_pitch                    20                              /* Integral pitch gain */
#define Kd_pitch                    11.19                           /* Derivative pitch gain */
#define Kp_yaw                      156                             /* Proportional yaw gain */
#define Ki_yaw                      0.1475                          /* Integral yaw gain */
#define Kd_yaw                      3.73                            /* Derivative yaw gain */
#define Kp_roll_dot                 468                             /* Proportional roll derivate gain */
#define Ki_roll_dot                 0.7375                          /* Integral roll derivate gain */
#define Kd_roll_dot                 18.65                           /* Derivative roll derivate gain */
#define Kp_pitch_dot                250                             /* Proportional pitch derivate gain */
#define Ki_pitch_dot                50                              /* Integral pitch derivate gain */
#define Kd_pitch_dot                111.9                           /* Derivative pitch derivate gain */
#define Kp_yaw_dot                  624                             /* Proportional yaw derivate gain */
#define Ki_yaw_dot                  1.475                           /* Integral yaw derivate gain */
#define Kd_yaw_dot                  149.2                            /* Derivative yaw derivate gain */
#define Z_FREQUENCY                 10                              /* Low Pass Filter z Cut Off Frequency */
#define ROLL_FREQUENCY              30                              /* Low Pass Filter Roll Cut Off Frequency */
#define PITCH_FREQUENCY             50                              /* Low Pass Filter Pitch Cut Off Frequency */
#define YAW_FREQUENCY               50                              /* Low Pass Filter Yaw Cut Off Frequency */
#define ROLL_DOT_FREQUENCY          1                               /* Low Pass Filter Roll Derivate Cut Off Frequency */
#define PITCH_DOT_FREQUENCY         0.1                             /* Low Pass Filter Pitch Derivate Cut Off Frequency */
#define YAW_DOT_FREQUENCY           0.0001                          /* Low Pass Filter Yaw Derivate Cut Off Frequency */
#define NEW_DEV_CONN_GPIO           17                              /* New device connected LED */
#define LOAD_BYTES_GPIO             16                              /* Load bytes to flash memory LED */
#define ERASE_BYTES_GPIO            4                               /* Erase bytes from Flash memory LED */
#define SCL_GPIO                    22                              /* SCL GPIO used to communicate with mpu9250 */
#define SDA_GPIO                    21                              /* SDA GPIO used to communicate with mpu9250 */
#define N_CONTROL_VARIABLES         7                               /* Variables to control */
#define N_MOTORS                    4                               /* Motors used by the dron */



/* ******************************************
                OBJECT STRUCTS
   ****************************************** */

/**
 * @brief dron_pwm_cfg_t struct used to configure Pwm object.
 * @param m1: Motor 1 configurations.
 * @param m2: Motor 2 configurations.
 * @param m3: Motor 3 configurations.
 * @param m4: Motor 4 configurations.
 */
typedef struct pwm_params
{
    pwm_cfg_t m1;
    pwm_cfg_t m2;
    pwm_cfg_t m3;
    pwm_cfg_t m4;
}pwm_params_t;

/**
 * @brief bt_cfg struct used to configure Bt_evt object.
 * @param new_device: New device LED ( Blue ) GPIO.
 * @param load_bytes: Loading bytes to Flash memory LED ( Green ) GPIO.
 * @param erase_bytes: Erasing bytes from Flash memory LED ( Red ) GPIO.
 */
typedef struct bt_cfg
{
    int new_device;
    int load_bytes;
    int erase_bytes;
}bt_params_t;

/**
 * @brief ctrler_cfg_t struct used to configure Pid object.
 * @param z_gains: pid_gain_t struct with z gains.
 * @param roll_gains: pid_gain_t struct with roll gains.
 * @param pitch_gains: pid_gain_t struct with pitch gains.
 * @param yaw_gains: pid_gain_t struct with yaw gains.
 * @param roll_dot_gains: pid_gain_t struct with roll derivate gains
 * @param pitch_dot_gains: pid_gain_t struct with pitch derivate gains
 * @param yaw_dot_gains: pid_gain_t struct with yaw derivate gains
 */
typedef struct ctrler_params
{
    pid_gain_t z_gains;
    pid_gain_t roll_gains;
    pid_gain_t pitch_gains;
    pid_gain_t yaw_gains;
    pid_gain_t roll_dot_gains;
    pid_gain_t pitch_dot_gains;
    pid_gain_t yaw_dot_gains;
}ctrler_params_t;

/**
 * @brief spiffs_cfg_t struct used to configure Spiffs object.
 * @param cfg: esp_vfs_spiffs_conf_t configuration struct.
 * @param filenames: Pointer to array of strings.
 * @param n_files: Total of strings.
 */
typedef struct spiffs_params_t
{
    esp_vfs_spiffs_conf_t cfg;
    char ** filenames;
    int n_files;
}spiffs_params_t;

/**
 * @brief mpu9250_cfg_t struct used to configure Mpu9250 object.
 * @param i2c_cfg: i2c configurations.
 * @param acc_range: Accelerometer range.
 * @param gyro_range: Gyroscope range.
 * @param ak8963_mode: AK8963 mode.
 * @param ak8963_bit: AK8963 bit??.
 * @param mag_declination: Magnetic declination.
 * @param gyro_offset: Gyroscope offsets ( x, y and z ).
 */
typedef struct mpu9250_params
{
    i2c_config_t i2c_cfg;
    int acc_range;
    int gyro_range;
    int ak8963_mode;
    int ak8963_bit;
    float mag_declination;
    offset_t gyro_offset;
}mpu9250_params_t;

/**
 * @brief hcsr04_cfg_t struct used to configure Hcsr04 object.
 * @param trig: Trig GPIO.
 * @param echo: Echo GPIO.
 */
typedef struct hcsr04_params
{
    int trig;
    int echo;
}hcsr04_params_t;

/**
 * @brief dron_cfg_t struct used to configure a Dron object.
 * @param hcsr04: hcsr04 configurations.
 */
typedef struct dron_cfg
{
    hcsr04_params_t hcsr04_cfg;
    mpu9250_params_t mpu9250_cfg;
    spiffs_params_t spiffs_cfg;
    ctrler_params_t pid_controller_cfg;
    bt_params_t bt_cfg;
    const uint8_t * flight_controller_mac;
    pwm_params_t pwm_cfg;
}dron_cfg_t;

typedef struct setpoints
{
    float z;
    float roll;
    float pitch;
    float yaw;
}setpoints_t;

/**
 * @brief states_t struct used to define the state variables of the dron ( x, roll, pitch and yaw ).
 * @param z: z state.
 * @param roll: Roll state.
 * @param pitch: Pitch state.
 * @param yaw: Yaw state.
 * @param roll_dot: Roll Derivate state.
 * @param pitch_dot: Pitch Derivate state.
 * @param yaw_dot: Yaw Derivate state.
 */
typedef struct states
{
    float z        [ 2 ];
    float roll     [ 2 ];
    float pitch    [ 2 ];
    float yaw      [ 2 ];
    float roll_dot [ 2 ];
    float pitch_dot[ 2 ];
    float yaw_dot  [ 2 ];
}states_t;

/**
 * @brief components_t struct used to access all components
 * @param hcsr04: hcsr04_t struct used to access object attributes and methods.
 * @param pwm: pwm_t struct used to access object attributes and methods.
 * @param flight_controller: joystick_t struct used to access object attributes and methods.
 * @param spiffs: spiffs_t struct used to access object attributes and methods.
 * @param controller: pid_controller_t struct used to access object attributes and methods.
 * @param bt: bt_evt_t struct used to access object attributes and methods.
 * @param mma: mma_t struct used to access object attributes and methods.
 * @param mpu: mpu9250_t struct used to access object attributes and methods.
 */
typedef struct components
{
    hcsr04_t hcsr04;
    pwm_t pwm[ N_MOTORS ];
    joystick_t flight_controller;
    spiffs_t spiffs;
    pid_controller_t controller[ N_CONTROL_VARIABLES ];
    bt_evt_t bt;
    mma_t mma;
    mpu9250_t mpu;
}components_t;

typedef struct dron dron_t;     /* Forward declaration neccesary, in compilation time, by update_t and measure_t structs */

/**
 * @brief update_t struct encharged of updating Dron states.
 */
typedef struct update
{
    /**
     * @brief Update z state of Dron object.
     * @param self: Pointer to dron_t struct.
     */
    void ( *distance )( dron_t * self );

    /**
     * @brief Update Roll, Pitch and Yaw states of Dron object.
     * @param self: Pointer to dron_t struct.
     */
    void ( *angles )( dron_t * self );
}update_t;

/**
 * @brief measure_t struct encharged of measuring Dron object states.
 * @param distance: Method used to measure distance.
 * @param angles: Method used to measure angles.
 */
typedef struct measure
{
    /**
     * @brief Measure actual distance of Dron object
     * @param self: Pointer to update_t struct.
     */
    void ( *distance )( dron_t * self );

    /**
     * @brief Measure angular accelerations of Dron object
     * @param self: Pointer to update_t struct.
     */
    void ( *angles )( dron_t * self );
}measure_t;

/**
 * @brief dron_t struct used to make an instance of Dron class.
 * @param states: satates_t struct used to access Dron states variables.
 * @param components: components_t struct used to access Dron internal objects attributes and methods.
 */
typedef struct dron
{
    /**
     * Attributes
     * ----------
     * 
     */
    states_t states;                            /* Variable used to access Dron states (z, Roll, Pitch and Yaw) */
    setpoints_t sp;                             /* Set Point of each state */
    components_t components;                    /* Variable used to access Dron internal objects */

    /**
     * Methods
     * -------
     * 
     */
    measure_t measure;          /* Variable used to acces measuring methods */
    update_t update;            /* Variable used to access updating methods */
}dron_t;



/* **********************************************
                FUNCTIONS PROTOTYPES
   ********************************************** */

/**
 * @brief Update z state attribute of Dron object.
 * @param self: Pointer to dron_t struct.
 */
void update_distance( dron_t * self );

/**
 * @brief Update Roll, Pitch and Yaw states of Dron object.
 * @param self: Pointer to dron_t struct.
 */
void update_angles( dron_t * self );

/**
 * @brief Measure duration of Echo signal.
 * @param self: Pointer to dron_t struct.
 */
void measure_distance( dron_t * self );

/**
 * @brief Measure Roll, Pitch and Yaw rates and update mpu internal registers.
 * @param self: Pointer to dron_t struct.
 */
void measure_angles( dron_t * self );

#endif
