#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "include/ps3.h"
#include "include/ps3_int.h"
#include "include/bt_int.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"

#define PS3_TAG                 "PS3_SPP"
#define PS3_DEVICE_NAME         "PS3 Host"
#define PS3_SERVER_NAME         "PS3_SERVER"

bt_flags_t flag;                /* Used to handle Bluetooth events */

/********************************************************************************/
/*              L O C A L    F U N C T I O N     P R O T O T Y P E S            */
/********************************************************************************/

static void ps3_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

/********************************************************************************/
/*                      P U B L I C    F U N C T I O N S                        */
/********************************************************************************/

/*******************************************************************************
**
** Function         ps3_spp_init
**
** Description      Initialise the SPP server to allow to be connected to
**
** Returns          void
**
*******************************************************************************/
void ps3_spp_init()
{
    esp_err_t ret;

    ESP_ERROR_CHECK( esp_bt_controller_mem_release( ESP_BT_MODE_BLE ) );

#ifndef ARDUINO_ARCH_ESP32
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_enable(BT_MODE)) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
#endif

    if ((ret = esp_spp_register_callback(ps3_spp_callback)) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s spp register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    esp_spp_cfg_t bt_spp_cfg = {
        .mode = ESP_SPP_MODE_CB,
        .enable_l2cap_ertm = true,
        .tx_buffer_size = 0,
    };
    if ((ret = esp_spp_enhanced_init(&bt_spp_cfg)) != ESP_OK) {                             /* esp_spp_init is deprecated */
        ESP_LOGE(PS3_TAG, "%s spp init failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
}


/*******************************************************************************
**
** Function         ps3_spp_deinit
**
** Description      Deinitialise the SPP server
**
** Returns          void
**
*******************************************************************************/
void ps3_spp_deinit()
{

    esp_err_t ret;

    if ((ret = esp_spp_deinit()) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s spp deinit failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

#ifndef ARDUINO_ARCH_ESP32
    if ((ret = esp_bluedroid_disable()) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s disable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_deinit()) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s deinitialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_disable()) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s disable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_deinit()) != ESP_OK) {
        ESP_LOGE(PS3_TAG, "%s deinitialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
#endif
}



/********************************************************************************/
/*                      L O C A L    F U N C T I O N S                          */
/********************************************************************************/

/*******************************************************************************
**
** Function         ps3_spp_callback
**
** Description      Callback for SPP events, only used for the init event to
**                  configure the SPP server
**
** Returns          void
**
*******************************************************************************/
static void ps3_spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch ( event )
    {
    case ESP_SPP_INIT_EVT:
        if ( param->init.status == ESP_SPP_SUCCESS )
        {
            ESP_LOGI( PS3_TAG, "ESP_SPP_INIT_EVT" );
            esp_spp_start_srv( ESP_SPP_SEC_NONE, ESP_SPP_ROLE_SLAVE, 0, PS3_SERVER_NAME );
        }
        break;
    case ESP_SPP_START_EVT:
        if ( param->start.status == ESP_SPP_SUCCESS )
        {
            ESP_LOGI( PS3_TAG, "ESP_SPP_START_EVT" );
            esp_bt_dev_set_device_name( PS3_DEVICE_NAME );
            esp_bt_gap_set_scan_mode( ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE );
        }
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        flag.new_dev = 1;                                                                                   /* New device connected Flag ON */
        ESP_LOGI( PS3_TAG, "New device connected, MAC Address: %02x:%02x:%02x:%02x:%02x:%02x",
        param->srv_open.rem_bda[ 0 ], param->srv_open.rem_bda[ 1 ], param->srv_open.rem_bda[ 2 ],
        param->srv_open.rem_bda[ 3 ], param->srv_open.rem_bda[ 4 ], param->srv_open.rem_bda[ 5 ] );
        break;
    case ESP_SPP_CLOSE_EVT:
        flag.new_dev = 0;                                                                                   /* New device connected Flag OFF */
        ESP_LOGI( PS3_TAG, "A device has disconnected, MAC Address: %02x:%02x:%02x:%02x:%02x:%02x",
        param->srv_open.rem_bda[ 0 ], param->srv_open.rem_bda[ 1 ], param->srv_open.rem_bda[ 2 ],
        param->srv_open.rem_bda[ 3 ], param->srv_open.rem_bda[ 4 ], param->srv_open.rem_bda[ 5 ] );
        break;
    case ESP_SPP_DATA_IND_EVT:                                                                              /* New data received */
        esp_spp_write( param->data_ind.handle, param->data_ind.len, param->data_ind.data );                 /* Echo: transmit back to device the data received */
        flag.data_received = 1;                                                                             /* Data received Flag ON */
        strncpy( flag.data, (char * ) param->data_ind.data, sizeof( flag.data ) - 1 );                      /* Load data received to variable */
        flag.data[ sizeof( flag.data ) - 1 ] = '\0';                                                        /* Make sure string ends with '\0' */
        break;
    default:
        break;
    }
}
