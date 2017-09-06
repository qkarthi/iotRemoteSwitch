/*
   General options
*/

#define BOARD_FIRMWARE_VERSION        "1.0.1"         //(contact project admin).
#define BOARD_HARDWARE_VERSION        "1.0.0"         //(contact project admin).

#define BOARD_NAME                    "Smart Switch"  // Name of your product (contact project admin). Should match App Export request info.
#define BOARD_VENDOR                  "iotics"        // Name of your company (contact project admin). Should match App Export request info.

#define PRODUCT_WIFI_SSID             "Smart Switch"  // Name of the device (contact project admin), to be displayed during configuration. Should match export request info.
#define BOARD_CONFIG_AP_URL           "192.168.1.50"   // Config page will be available in a browser at 'http://192.168.1.50'.


  // Custom board configuration
#define BOARD_BUTTON_PIN            5                     // Pin where user button is attached
#define BOARD_BUTTON_ACTIVE_LOW     false                  // true if button is "active-low"
#define BOARD_BUZZER_PIN            4
/*
   IOTICS Configuration
*/

#define PRODUCT_SHORTCODE            "smartswitch-iot"  //This will be a uniquename without any spaces.
#define BOARD_ENV                    "2s_alpha"         //Product environment should be development or production.
#define OTA_URL                      "https://s3.ap-south-1.amazonaws.com/iotics-firmware"

/*
   Board configuration
*/

// reset button configuration
#define BOARD_RESET_BUTTON               true  // make it true if reset button is associated with the board.
#define BOARD_RESET_BUTTON_PIN           5     // pin number in esp chip format.
#define BOARD_RESET_BUTTON_ACTIVE_LOW    false // pullup -> false ..... pulldown -> true.

// led configuration
#define BOARD_LED_R             false         // make it true if RED LED is associated with the board.
#define BOARD_LED_PIN_R         D8            // pin number in esp chip format.
#define BOARD_LED_G             false         // make it true if GREEN LED is associated with the board.
#define BOARD_LED_PIN_G         D7            // pin number in esp chip format.
#define BOARD_LED_B             false         // make it true if BLUE LED is associated with the board.
#define BOARD_LED_PIN_B         D6            // pin number in esp chip format.

#define BOARD_LED_INVERSE       false     // normal association -> false ..... pulldown association -> true.
#define BOARD_RGB_BRIGHTNESS    32        // level of brightness.

/*
   Advanced options
*/

#define BUTTON_HOLD_TIME_INDICATION   3000   // time interval for indicator (R/B/G) to blink
#define BUTTON_HOLD_TIME_ACTION       10000  // time interval for hold down to reset

#define BOARD_PWM_MAX                 1023  // pwm for indicators.

#define WIFI_NET_CONNECT_TIMEOUT      30000 // wifi-connect time out
#define WIFI_CLOUD_CONNECT_TIMEOUT    15000 // blynk time out 
#define WIFI_AP_CONFIG_PORT           80    // port for web configuration ( 80 for default http - web )
#define WIFI_AP_IP                    IPAddress(192, 168, 1, 50)  // IP address for clients
#define WIFI_AP_Subnet                IPAddress(255, 255, 255, 0)  // IP Subnet for clients
//#define WIFI_CAPTIVE_PORTAL_ENABLE

#define USE_TICKER
//#define USE_TIMER_ONE
//#define USE_TIMER_THREE

#if defined(APP_DEBUG)
#define DEBUG_PRINT(...) BLYNK_LOG1(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

