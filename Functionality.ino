////////////////////////////////////////////////////////////////////////////////////
BLYNK_CONNECTED() {
  DEBUG_PRINT("INFO : blynk connected");
  FTC = false;
  rtc.begin();

  if (DFConfig) {
    d1_state = LOW;
    d2_state = LOW;
    DFConfig = false;
    d1_hw_switch_1_state = digitalRead(d1_hw_switch_pin_1); // getting the state of hardware device 1 switch
    d2_hw_switch_1_state = digitalRead(d2_hw_switch_pin_1); // getting the state of hardware device 2 switch

  }

  if (d1_state)
  {
    Blynk.virtualWrite(V_D1ManButPin, HIGH);
    d1_led.on();
  }
  else  {
    Blynk.virtualWrite(V_D1ManButPin, LOW);
    d1_led.off();
  }

  if (d2_state)
  {
    Blynk.virtualWrite(V_D2ManButPin, HIGH);
    d2_led.on();
  }
  else  {
    Blynk.virtualWrite(V_D2ManButPin, LOW);
    d2_led.off();
  }
  Blynk.syncVirtual(V21);
  Blynk.syncVirtual(V22);
  Blynk.syncVirtual(V71);
  Blynk.syncVirtual(V72);
  Blynk.syncVirtual(V6);
  Blynk.syncVirtual(V7);
  comb_Dstatus();
}
BLYNK_DISCONNECTED() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("+2");
  } else {
    Serial.println("+1");
  }
  DEBUG_PRINT("INFO : blynk disconnected");
}
BLYNK_WRITE(V6) // for upnp
{
  configStore.upnp_vars = param.asInt();
  config_save();
  if (!configStore.upnp_vars) {
    UDP.stop();
    HTTP1.stop();
    HTTP2.stop();
  } else {
    alexa_initiated = false;
  }
  Serial.println(configStore.upnp_vars);
}
BLYNK_WRITE(V11) // manual button for d1
{
  d1_state  = param.asInt();
  d1_update_func(0, 0);
}
BLYNK_WRITE(V12) // manual button for d2
{
  d2_state  = param.asInt();
  d2_update_func(0, 0);
}

BLYNK_WRITE(V7) // manual button for D2
{
  CopyString(param.asStr(), configStore.device_1_name);
  config_save();// call the function inorder to save the eeprom data of maodified records
  Blynk.setProperty(V_D1ManButPin, "label",  configStore.device_1_name);
  Blynk.setProperty(V_DevSelector1, "labels", configStore.device_1_name, configStore.device_2_name);
  Blynk.setProperty(V_DevSelector2, "labels", configStore.device_1_name, configStore.device_2_name);
}
BLYNK_WRITE(V8) // manual button for D2
{
  CopyString(param.asStr(), configStore.device_2_name);
  config_save();// call the function inorder to save the eeprom data of maodified records
  Blynk.setProperty(V_D2ManButPin, "label",  configStore.device_2_name);
  Blynk.setProperty(V_DevSelector1, "labels", configStore.device_1_name, configStore.device_2_name);
  Blynk.setProperty(V_DevSelector2, "labels", configStore.device_1_name, configStore.device_2_name);
}
////////////////////////////////////////////////////////
BLYNK_WRITE(V99) // terminal widget
{
  TermString = param.asStr();
  //Serial.println(TermString);
  TermCharOne = TermString.charAt(0);
  TermCharRest = TermString.substring(1);

  if (String("s") == TermCharOne & (String("uccess") == TermCharRest)) {
    /////////
  } else if ((String("f") == TermCharOne) & (String("actory_reset") == TermCharRest) ) {
    terminal.println(" ");
    terminal.println("factory reset will initiated in 3 seconds");
    terminal.flush();
    terminal.println("success");
    delay(3000);
    enterResetConfig();  // reset esp after 3 seconds .
  } else if ((String("r") == TermCharOne) & (String("eboot") == TermCharRest)) {
    terminal.println(" ");
    terminal.println("Device reboots in 3 seconds");
    terminal.flush();
    delay(3000);
    ESP.restart();
  } else  if ((String("i") == TermCharOne) & (String("p_address") == TermCharRest) ) {
    terminal.println(" ");
    terminal.println(WiFi.localIP().toString());
  } else if ((String("t") == TermCharOne) & (String("ime") == TermCharRest)) {
    terminal.println(" ");
    TermTimeStr = String(hour()) + ":" + String(minute()) + ":" + String(second()) + "-" + String(day()) + "/" + String(month()) + "/" + String(year());
    terminal.println(TermTimeStr);
  } else if ((String("m") == TermCharOne) & (String("ac") == TermCharRest)) {
    terminal.println(client_id);
  } else if ((String("s") == TermCharOne) & (String("sid") == TermCharRest)) {
    terminal.println(configStore.wifiSSID);
  } else if ((String("c") == TermCharOne) & (String("loud") == TermCharRest)) {
    terminal.println(configStore.cloudHost);
  } else if ((String("c") == TermCharOne) & (String("loud_token") == TermCharRest)) {
    terminal.println(configStore.cloudToken);
  } else if ((String("f") == TermCharOne) & (String("irmware") == TermCharRest)) {
    terminal.println(CURRENT_FIRMWARE);
  } else if ((String("o") == TermCharOne) & (String("ta") == TermCharRest)) {
    terminal.println(PRODUCT_SHORTCODE);
  } else if ((String("u") == TermCharOne) & (String("pgrade") == TermCharRest)) {
    terminal.println("firmware upgrade initiated");
    terminal.flush();
    firmware_update();
  } else if (String("1") == TermCharOne) {
    Blynk.virtualWrite(V_D1Label, TermCharRest);
    CopyString(TermCharRest, configStore.device_1_name);
    config_save();// call the function inorder to save the eeprom data of maodified records
    Blynk.setProperty(V_D1ManButPin, "label", TermCharRest);
    Blynk.setProperty(V_DevSelector1, "labels", configStore.device_1_name, configStore.device_2_name);
    Blynk.setProperty(V_DevSelector2, "labels", configStore.device_1_name, configStore.device_2_name);
    terminal.println("success");
  }  else if (String("2") == TermCharOne) {
    Blynk.virtualWrite(V_D2Label, TermCharRest);
    CopyString(TermCharRest, configStore.device_2_name);
    config_save(); // call the function inorder to save the eeprom data of maodified records
    Blynk.setProperty(V_D2ManButPin, "label", TermCharRest);
    Blynk.setProperty(V_DevSelector1, "labels", configStore.device_1_name, configStore.device_2_name);
    Blynk.setProperty(V_DevSelector2, "labels", configStore.device_1_name, configStore.device_2_name);
    terminal.println("success");
  }
  terminal.flush();
}
////////////////////////////////////////////////
void firmware_update() {
  overTheAirURL = OTA_URL + String("/") + BOARD_ENV + String("/") + PRODUCT_SHORTCODE + String("/firmware.bin");
  Blynk.disconnect();
  BlynkState::set(MODE_OTA_UPGRADE);
  delay(500);
}
////////////////////////////////////////////////
void d1_update_func(int hw_valid, int hw_switch_state) {
  digitalWrite(d1_op_pin_1, d1_state);
  deviceLastMem(1, d1_state, hw_valid, hw_switch_state);
  if (d1_state == HIGH)
  {
    d1_led.on();
  } else
  {
    d1_led.off();
  }
  comb_Dstatus();
}
////////////////////////////////////
void d2_update_func(int hw_valid, int hw_switch_state ) {
  digitalWrite(d2_op_pin_1, d2_state);
  deviceLastMem(2, d2_state, hw_valid, hw_switch_state);
  if (d2_state == HIGH)
  {
    d2_led.on();
  } else
  {
    d2_led.off();
  }
  comb_Dstatus();
}
////////////////////////////////////
void upd_d1_hw_switch_1_func()
{
  if (digitalRead(d1_hw_switch_pin_1) != d1_hw_switch_1_state)
  {
    d1_hw_switch_1_state = !d1_hw_switch_1_state;
    d1_state = !d1_state;
    Blynk.virtualWrite(V_D1ManButPin, d1_state);
    d1_update_func(1, d1_hw_switch_1_state);
  }
}
///////////////////////////////
void upd_d2_hw_switch_1_func()
{
  if (digitalRead(d2_hw_switch_pin_1) != d2_hw_switch_1_state)
  {
    d2_hw_switch_1_state = !d2_hw_switch_1_state;
    d2_state = !d2_state;
    Blynk.virtualWrite(V_D2ManButPin, d2_state);
    d2_update_func(1, d2_hw_switch_1_state);
  }
}

///////////////////////////////
void deviceLastMem(int dev_id, int Dstatus, int hw_swit_valid, int hw_swit_state) {
  if (dev_id == 1) {
    configStore.Dev_1_state = Dstatus;
    if (hw_swit_valid) {
      configStore.Dev_1_swi = hw_swit_state;

    }
  } else if (dev_id == 2) {
    configStore.Dev_2_state = Dstatus;
    if (hw_swit_valid) {
      configStore.Dev_2_swi = hw_swit_state;
    }
  }
  config_save();
}
////////////////////////////////
void scheduler_1_func()
{
  Blynk.syncVirtual(V51); // time input widget for scheduling 1
}

void scheduler_2_func()
{
  Blynk.syncVirtual(V52); // time input widget for scheduling 2
}
//////////////////////////////
void con_maint_func() {
  if (BlynkState::is(MODE_DUMB)) {
    WiFi.mode(WIFI_OFF);
    enterConnectNet();
  }
  if (BlynkState::is(MODE_RUNNING)) {
    if (WiFi.status() != 3) {
      DEBUG_PRINT("ERROR : No WiFi for connection ");
      WiFi.mode(WIFI_OFF);
      enterConnectNet();
    }

    if ((WiFi.status() == WL_CONNECTED) && (!Blynk.connected()))
    {
      DEBUG_PRINT("connected to wifi ");
      DEBUG_PRINT(WiFi.SSID().c_str());

      DEBUG_PRINT("INFO : Disconnecting blynk in conmaintain func ");
      Blynk.disconnect();
      Blynk.config(configStore.cloudToken, configStore.cloudHost, configStore.cloudPort);
      Blynk.connect(0);

      unsigned long startConnecting = millis();
      while (!Blynk.connected()) {
        Blynk.connect();
        if (millis() > startConnecting + 1000) {
          DEBUG_PRINT("ERROR : Unable to connect to server. ");
          break;
        }
      }
    }
  }
}
//////////////////////////////////
void comb_Dstatus() {
  if ((d1_state == LOW) && (d2_state == LOW)) {
    Blynk.virtualWrite(V_DStaStr, lightOff_str + lightOff_str) ;
  } else if ((d1_state == LOW) && (d2_state == HIGH)) {
    Blynk.virtualWrite(V_DStaStr, lightOff_str + lightOn_str) ;
  } else if ((d1_state == HIGH) && (d2_state == LOW)) {
    Blynk.virtualWrite(V_DStaStr, lightOn_str + lightOff_str) ;
  } else if ((d1_state == HIGH) && (d2_state == HIGH)) {
    Blynk.virtualWrite(V_DStaStr, lightOn_str + lightOn_str) ;
  }
}
//////////////////////////////////
void upd_equipment_detail_func()
{
  Blynk.virtualWrite(V_RssiPin,  WiFi.RSSI() + 120); // 120 for tolerance to make the value in positive
}


