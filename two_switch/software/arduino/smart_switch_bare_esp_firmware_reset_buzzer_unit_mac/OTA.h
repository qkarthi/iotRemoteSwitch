#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

String overTheAirURL;

//This function will use when our code moved to prod environment
BLYNK_WRITE(InternalPinOTA) {
  //URL should be something like "http://ota-server:port/firmware.bin?auth=12345"
  overTheAirURL = param.asString();
  overTheAirURL += String("http://blynk.fslab.in:3000/firmware.bin?auth=") + configStore.cloudToken;

  // Disconnect, not to interfere with OTA process
  Blynk.disconnect();

  // Start OTA
  BlynkState::set(MODE_OTA_UPGRADE);
  delay(500);
}

//For development mode
BLYNK_WRITE(V111) {
   int pinValue = param.asInt();
   if (pinValue == 1) {
      overTheAirURL = OTA_URL+String("/")+BOARD_ENV+String("/")+PRODUCT_SHORTCODE+String("/firmware.bin");
   
   // Disconnect, not to interfere with OTA process
  Blynk.disconnect();

  // Start OTA
  BlynkState::set(MODE_OTA_UPGRADE);
  delay(500);
  }
}
 

void enterOTA() {
  BlynkState::set(MODE_OTA_UPGRADE);

  DEBUG_PRINT(String("Firmware update URL: ") + overTheAirURL);
  switch (ESPhttpUpdate.update(overTheAirURL, BOARD_FIRMWARE_VERSION)) {
  case HTTP_UPDATE_FAILED:
    DEBUG_PRINT(String("Firmware update failed (error ") + ESPhttpUpdate.getLastError() + "): " + ESPhttpUpdate.getLastErrorString());
    restartMCU();
    break;
  case HTTP_UPDATE_NO_UPDATES:
    DEBUG_PRINT("No firmware updates available.");
    BlynkState::set(MODE_CONNECTING_CLOUD);
    break;
  case HTTP_UPDATE_OK:
    DEBUG_PRINT("Firmware updated.");
    restartMCU();
    break;
  }
}