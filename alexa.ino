void alexa_device_rename() {
  fauxmo.renameDevice(0 , configStore.mspace_1); // this function used to rename device name
  fauxmo.renameDevice(1 , configStore.mspace_2); // this function used to rename device name
  Serial.println("rename");
}

void alexaDevAdd() {
  //checks wheather the wifi connected or not and if it is connected it makes the validator varaiable to true
  if ( (WiFi.status() == WL_CONNECTED) && (alexaDevAddedFlag == false)) {
    fauxmo.addDevice(configStore.mspace_1);
    delay(100);
    fauxmo.addDevice(configStore.mspace_2);
    alexaDevAddedFlag = true;
    Serial.println("added");
  }
}

void callback(uint8_t DevId, const char * device_name, bool state) {
  // Notes : below codes checks if the device is on or off and makes the trigger
  if ( (strcmp(device_name, configStore.mspace_1) == 0) ) {
    if (state) {
      d1_state  =HIGH;
    }  else {
      d1_state  =LOW;
    }
     d1_update_func();
  }

  if ( (strcmp(device_name, configStore.mspace_2) == 0) ) {
    if (state) {
      d2_state  =HIGH;
    }
    else {
    d2_state  =LOW;
    }
     d2_update_func();
  }
}
