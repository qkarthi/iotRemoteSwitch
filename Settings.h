/*
   General options
*/

#define BOARD_FIRMWARE_VERSION        "1.1.0"
#define BOARD_HARDWARE_VERSION        "1.1.0"

#define BOARD_NAME                    "XXXXX"           // Name of your product. Should match App Export request info.
#define BOARD_VENDOR                  "XXXXX"           // Name of your company. Should match App Export request info.
#define BOARD_TEMPLATE_ID             "XXXXX"              
                                                               // PRODUCTION VERSION template .

#define PRODUCT_WIFI_SSID             "SW_V103"             // Name of the device, to be displayed during configuration. Should match export request info.
#define BOARD_CONFIG_AP_URL           "switcher.set"            // Config page will be available in a browser at 'http://our-product.cc/'

#define BOARD_BUTTON_PIN            16
#define BOARD_BUTTON_ACTIVE_LOW     false
#define BAUD_RATE                   9600
/*
   IOTICS Configuration
*/

#define PRODUCT_SHORTCODE            "switcher/io_sw_2d_v_1_0"               //This will be a uniquename without any spaces.
#define BOARD_ENV                    "development"                                  //Product environment should be development or production.
#define OTA_URL                      "http://firmware.s3.amazonaws.com"    //please check it if OTA not works.
//#define CURRENT_FIRMWARE             "io_sw_2d_v_1_0_3_11_06_2018_debug_v_1"                //Firware version will start from v.0.1 
#define CURRENT_FIRMWARE             "io2sw_v010-1607201819"

/*
   Advanced options
*/

#define BUTTON_HOLD_TIME_INDICATION   3000
#define BUTTON_HOLD_TIME_ACTION       10000

#define BOARD_PWM_MAX                 1023
////////////////////////////////////////////////////////////////////////////
#define WIFI_NET_CONNECT_TIMEOUT      10000 //<<-----<< CHECK HERE IF NETCONNECT ERROR (note : increase time)
////////////////////////////////////////////////////////////////////////////
#define WIFI_CLOUD_CONNECT_TIMEOUT    15000
#define WIFI_AP_CONFIG_PORT           80
#define WIFI_AP_IP                    IPAddress(192, 168, 4, 1)
#define WIFI_AP_Subnet                IPAddress(255, 255, 255, 0)
//#define WIFI_CAPTIVE_PORTAL_ENABLE

#define USE_TICKER
//#define USE_TIMER_ONE
//#define USE_TIMER_THREE

#if defined(APP_DEBUG)
#define DEBUG_PRINT(...) BLYNK_LOG1(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif
