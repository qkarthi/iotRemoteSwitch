/*
   General options
*/
#define BOARD_FIRMWARE_VERSION        "1.0.4"
#define BOARD_HARDWARE_VERSION        "1.0.0"

#define BOARD_NAME                    "Switcher"         // Name of your product. Should match App Export request info.
#define BOARD_VENDOR                  "iotics"               // Name of your company. Should match App Export request info.

#define PRODUCT_WIFI_SSID             "Switcher"    // mac id also displayed -  Name of the device, to be displayed during configuration. Should match export request info.
#define BOARD_CONFIG_AP_URL           "Switcher.set"      // Config page will be available in a browser at 'http://our-product.cc/'

/*
   IOTICS Configuration
*/
#define PRODUCT_SHORTCODE            "switcher/alpha"     //This will be a uniquename without any spaces.
#define BOARD_ENV                    "development"          //Product environment should be development or production.
#define OTA_URL                      "http://iotics-c-firmware.s3.amazonaws.com"
                                      //"http://iotics-c-firmware.s3.amazonaws.com"

// Custom board configuration

#define BOARD_BUTTON_PIN            5                     // Pin where user button is attached
#define BOARD_BUTTON_ACTIVE_LOW     false                 // true if button is "active-low"
#define BOARD_BUZZER_PIN            4


/*
   Advanced options
*/

#define BUTTON_HOLD_TIME_INDICATION   3000
#define BUTTON_HOLD_TIME_ACTION       10000

#define BOARD_PWM_MAX                 1023

#define WIFI_NET_CONNECT_TIMEOUT      30000
#define WIFI_CLOUD_CONNECT_TIMEOUT    15000
#define WIFI_AP_CONFIG_PORT           80
#define WIFI_AP_IP                    IPAddress(192, 168, 1, 50)
#define WIFI_AP_Subnet                IPAddress(255, 255, 255, 0)

#define USE_TICKER

#if defined(APP_DEBUG)
#define DEBUG_PRINT(...) BLYNK_LOG1(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

