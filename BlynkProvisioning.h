/**************************************************************
   This is a DEMO. You can use it only for development and testing.

   If you would like to add these features to your product,
   please contact Blynk for Business:

                    http://www.blynk.io/

 **************************************************************/

extern "C" {
#include "user_interface.h"
}
#include "HWPins.h"
#include "Settings.h"
#include "BlynkState.h"
#include "ConfigStore.h"
#include "ConfigMode.h"
#include "OTA.h"
#include "DumpMode.h"
#include "FBDumpMode.h"
inline
void BlynkState::set(State m) {
  if (state != m) {
    DEBUG_PRINT(String(StateStr[state]) + " => " + StateStr[m]);
    state = m;
  }
}

class Provisioning {

  public:
    void begin()
    {
      DEBUG_PRINT("");
      DEBUG_PRINT("Hardware v" + String(BOARD_HARDWARE_VERSION));
      DEBUG_PRINT("Firmware v" + String(BOARD_FIRMWARE_VERSION));

      randomSeed(ESP.getChipId());

      config_init();

      if (configStore.flagConfig) {
        BlynkState::set(MODE_CONNECTING_NET);
      } else {
        BlynkState::set(MODE_DUMB);
      }
    }

    void run() {
      switch (BlynkState::get()) {
        case MODE_DUMB:              enterDumbMode();      break;
        case MODE_WAIT_CONFIG:
        case MODE_CONFIGURING:       enterConfigMode();    break;
        case MODE_CONNECTING_NET:    enterConnectNet();    break;
        case MODE_CONNECTING_CLOUD:  enterConnectCloud();  break;
        case MODE_RUNNING:           Blynk.run();         break;
        case MODE_OTA_UPGRADE:       enterOTA();           break;
        case MODE_SWITCH_TO_STA:     enterSwitchToSTA();   break;
        case MODE_RESET_CONFIG:      enterResetConfig();   break;
        case MODE_FBDUMB:            FBDumbMode();         break;
        default:                     enterError();         break;
      }
    }

};

Provisioning BlynkProvisioning;

