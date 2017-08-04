/*
 * General options
 */

#define BOARD_FIRMWARE_VERSION        "1.0.1"
#define BOARD_HARDWARE_VERSION        "1.0.0"

#define BOARD_NAME                    "SmartSwitch"             // Name of your product. Should match App Export request info.
#define BOARD_VENDOR                  "fslab"                 // Name of your company. Should match App Export request info.

#define PRODUCT_WIFI_SSID             "SmartSwitch(2D)"      // Name of the device, to be displayed during configuration. Should match export request info.
#define BOARD_CONFIG_AP_URL           "smartswitch.set"      // Config page will be available in a browser at 'http://our-product.cc/'

/*
 * Board configuration (see examples below).
 */

#if defined(USE_CUSTOM_BOARD)
  // Custom board configuration
#else
  #error "No board selected"
#endif


/*
 * Advanced options
 */

#define BUTTON_HOLD_TIME_INDICATION   3000
#define BUTTON_HOLD_TIME_ACTION       10000

#define BOARD_PWM_MAX                 1023

#define WIFI_NET_CONNECT_TIMEOUT      30000
#define WIFI_CLOUD_CONNECT_TIMEOUT    15000
#define WIFI_AP_CONFIG_PORT           80
#define WIFI_AP_IP                    IPAddress(192, 168, 4, 1)
#define WIFI_AP_Subnet                IPAddress(255, 255, 255, 0)

#define USE_TICKER

#if defined(APP_DEBUG)
  #define DEBUG_PRINT(...) BLYNK_LOG1(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
#endif

