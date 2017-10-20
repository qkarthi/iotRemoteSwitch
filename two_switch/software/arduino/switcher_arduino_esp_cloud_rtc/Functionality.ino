////////////////////////////////////////////////////////////////////////////////////
BLYNK_CONNECTED() {
  Blynk.syncAll();   // used to sync all pins when the device is connected to server
  rtc.begin();
}
/////////////////////////////////////////////////////
BLYNK_WRITE(V8) // kwhr label
{
  k_watt_hr_value = param.asFloat();
}
BLYNK_WRITE(V9) // master off button for d1 and d2
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    if (k_watt_hr_value > k_watt_hr_value_reset_maximum)
    {
      Blynk.virtualWrite(vir_kwhr_previous_pin_1, k_watt_hr_value);
      date_string = "previous unit - reset on: " + String(day()) + "-" + String(month()) + "-" + String(year());
      Blynk.setProperty(vir_kwhr_previous_pin_1, "label", date_string);
      k_watt_hr_value = 0;
      Blynk.virtualWrite(vir_kwhr_pin_1, 0);
    }
  }
}
////////////////////////////////////////////////////
BLYNK_WRITE(V11) // manual button for d1
{
  d1_state  = param.asInt();
  if (d1_state == HIGH)
  {
    Serial.println("#d1onX");
  } else
  {
    Serial.println("#d1off");
  }
  d1_update_func();
}
BLYNK_WRITE(V12) // manual button for d2
{
  d2_state  = param.asInt();
  if (d2_state == HIGH)
  {
    Serial.println("#d2onX");
  } else
  {
    Serial.println("#d2off");
  }
  d2_update_func();
}
BLYNK_WRITE(V19) // master on button for d1 and d2
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    d1_state  = HIGH;
    d2_state  = HIGH;
    Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
    Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
    Serial.println("#aonXX");
    d1_update_func();
    delay(500);
    d2_update_func();
  }
}
BLYNK_WRITE(V20) // master off button for d1 and d2
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    d1_state  = LOW;
    d2_state  = LOW;
    Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
    Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
    d1_update_func();
    d2_update_func();
    Serial.println("#aoffX");
  }
}
////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V21) // scheduler 1 activation button
{
  scheduler_1_var_1  = param.asInt();
}
BLYNK_WRITE(V22) // scheduler 2 activation button
{
  scheduler_2_var_1  = param.asInt();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V51) // time input widget for scheduling 1
{
  if (scheduler_1_var_1 == HIGH) //scd_button selected
  {
    TimeInputParam t(param);
    nowseconds_var = ((hour() * 3600) + (minute() * 60) + second());
    dayadjustment_var = -1;
    if (weekday() == 1) // 1 as monday
    {
      dayadjustment_var =  6;
    }
    for (int day_i = 1; day_i <= 7; day_i++)
    {
      if (t.isWeekdaySelected(day_i))
      {
        if ((weekday() + dayadjustment_var) == day_i) // day selected
        {
          scheduler_1_start_seconds = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);
          scheduler_1_stop_seconds = (t.getStopHour() * 3600) + (t.getStopMinute() * 60) + additional_scheduling_intervals;
          if (((scheduler_1_start_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_1_start_seconds))
          {
            if (device_selected_scheduler_1 == 1)
            {
              d1_state = HIGH;
              d1_update_func(); //send values to devices for update
              Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
              Blynk.notify(String(configStore.mspace_1) + " powered on ");
            }
            if (device_selected_scheduler_1 == 2)
            {
              d2_state = HIGH;
              d2_update_func(); //send values to devices for update
              Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
              Blynk.notify(String(configStore.mspace_2) + " powered on ");
            }
          }
          /////////////////////////////////////////
          if ( ((scheduler_1_stop_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_1_stop_seconds))
          { if (device_selected_scheduler_1 == 1)
            {
              d1_state = LOW;
              d1_update_func(); //send values to devices for update
              Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);

              Blynk.notify(String(configStore.mspace_1) + " powered off ");
            }
            if (device_selected_scheduler_1 == 2)
            {
              d2_state = LOW;
              d2_update_func(); //send values to devices for update
              Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
              Blynk.notify(String(configStore.mspace_2) + " powered off ");
            }
          }
        }
      }
    }
  }
}

BLYNK_WRITE(V52) // time input widget for scheduling 2
{
  if (scheduler_2_var_1 == HIGH) //scd_button selected
  {
    TimeInputParam t(param);
    nowseconds_var = ((hour() * 3600) + (minute() * 60) + second());
    dayadjustment_var = -1;
    if (weekday() == 1) // 1 as monday
    {
      dayadjustment_var =  6;
    }
    for (int day_i = 1; day_i <= 7; day_i++)
    {
      if (t.isWeekdaySelected(day_i))
      {
        if ((weekday() + dayadjustment_var) == day_i) // day selected
        {
          scheduler_2_start_seconds = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);
          scheduler_2_stop_seconds = (t.getStopHour() * 3600) + (t.getStopMinute() * 60) + additional_scheduling_intervals;
          if (((scheduler_2_start_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_2_start_seconds))
          { if (device_selected_scheduler_2 == 1)
            {
              d1_state = HIGH;
              d1_update_func(); //send values to devices for update
              Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);

              Blynk.notify(String(configStore.mspace_1) + " powered on ");
            } if (device_selected_scheduler_2 == 2)
            {
              d2_state = HIGH;
              d2_update_func(); //send values to devices for update
              Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
              Blynk.notify(String(configStore.mspace_2) + " powered on ");
            }
          }
          /////////////////////////////////////////
          if ( ((scheduler_2_stop_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_2_stop_seconds))
          { if (device_selected_scheduler_2 == 1)
            {
              d1_state = LOW;
              d1_update_func(); //send values to devices for update
              Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
              Blynk.notify(String(configStore.mspace_1) + " powered off ");
            } if (device_selected_scheduler_2 == 2)
            {
              d2_state = LOW;
              d2_update_func(); //send values to devices for update
              Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
              Blynk.notify(String(configStore.mspace_2) + " powered off ");
            }
          }
        }
      }
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V71) // scheduler 1 device selecter
{
  device_selected_scheduler_1 = param.asInt();
}
BLYNK_WRITE(V72) // scheduler 2 device selecter
{
  device_selected_scheduler_2 = param.asInt();
}
////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V99) // terminal widget
{
  term_string = param.asStr();
  term_char_one = term_string.charAt(0);
  term_char_rest = term_string.substring(1);

  if (String("s") == term_char_one & (String("uccess") == term_char_rest)) {
    /////////
  } else  if ((String("f") == term_char_one) & (String("actory_reset") == term_char_rest) ) {
    terminal.println(" ");
    terminal.println("factory reset will initiated in 3 seconds");
    terminal.flush();
    Blynk.virtualWrite(99, "success");
    delay(3000);
    config_reset();
  } else  if ((String("i") == term_char_one) & (String("p_address") == term_char_rest) ) {
    terminal.println(" ");
    terminal.println(WiFi.localIP().toString());
  } else if ((String("r") == term_char_one) & (String("eboot") == term_char_rest)) {
    terminal.println(" ");
    terminal.println("Device reboots in 3 seconds");
    terminal.flush();
    delay(3000);
    ESP.restart();
  }  else if ((String("t") == term_char_one) & (String("ime") == term_char_rest)) {
    terminal.println("-");
    term_time_string = String(hour()) + ":" + String(minute()) + ":" + String(second());
    terminal.println(term_time_string);
  } else if ((String("b") == term_char_one) & (String("eep") == term_char_rest)) {
    terminal.println(" ");
    beep_var = 1;
    terminal.println("Beep sound will start within 10 seconds");
  } else if ((String("d") == term_char_one) & (String("isco") == term_char_rest)) {
    terminal.println(" ");
    disco_var = 1;
    terminal.println("disco will start within 10 seconds");
  } else if ((String("s") == term_char_one) & (String("os") == term_char_rest)) {
    terminal.println(" ");
    sos_var = 1;
    terminal.println("sos will start within 10 seconds");
  } else if ((String("e") == term_char_one) & (String("rror_error") == term_char_rest)) {
    terminal.println(" ");
    beep_var = 1;
    terminal.println("Beep sound will start within 10 seconds");
  } else  if ((String("*") == term_char_one)) {
    Blynk.virtualWrite(vir_equipment_name_pin_1, term_char_rest);
    CopyString(term_char_rest, configStore.equip_name);
    config_save();
    Blynk.virtualWrite(99,  configStore.equip_name);
    terminal.println("success");
  }  else if (String("1") == term_char_one) {
    Blynk.virtualWrite(vir_d1_label_pin_1, term_char_rest);
    CopyString(term_char_rest, configStore.mspace_1);
    config_save();
    Blynk.setProperty(vir_d1_man_button_pin_1, "label", term_char_rest);
    Blynk.setProperty(vir_device_selector_1, "labels", configStore.mspace_1, configStore.mspace_2);
    Blynk.setProperty(vir_device_selector_2, "labels", configStore.mspace_1, configStore.mspace_2);
    terminal.println("success");
  } else if (String("2") == term_char_one) {
    Blynk.virtualWrite(vir_d2_label_pin_1, term_char_rest);
    CopyString(term_char_rest, configStore.mspace_2);
    config_save();
    Blynk.setProperty(vir_d2_man_button_pin_1, "label", term_char_rest);
    Blynk.setProperty(vir_device_selector_1, "labels", configStore.mspace_1, configStore.mspace_2);
    Blynk.setProperty(vir_device_selector_2, "labels", configStore.mspace_1, configStore.mspace_2);
    terminal.println("success");
  }
  terminal.flush();
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
void con_manager_func()
{
  //DEBUG_PRINT("ver_2");
  if (WiFi.status() == WL_CONNECTED)
  {
    DEBUG_PRINT("wifi _ true ");
    if ((Blynk.connected()) == false)
    {
      DEBUG_PRINT("cloud _ false ");
      digitalWrite(BOARD_BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BOARD_BUZZER_PIN, LOW);
      delay(500);
      digitalWrite(BOARD_BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BOARD_BUZZER_PIN, LOW);
      //beep twice for 10 sec
    }
    else {
      DEBUG_PRINT("cloud _ true ");
    }
  }
  else
  {

    digitalWrite(BOARD_BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BOARD_BUZZER_PIN, LOW);

    //beep once for 10 sec
    DEBUG_PRINT("wifi _ false ");
    if (BlynkState::is(MODE_RUNNING) || BlynkState::is(MODE_CONNECTING_CLOUD)) {
      if (configStore.flagConfig) {
        BlynkState::set(MODE_CONNECTING_NET);
        DEBUG_PRINT("mode => connecting network ");
      }
    }
    // lets scan for available ssid
    int n = WiFi.scanNetworks();
    //if ssid found
    if (n > 0)
    {
      for (int i = 0; i < n; ++i)
      { // check weather the ssid match with saved ssid

        if (WiFi.SSID(i) == configStore.wifiSSID )
        {
          WiFi.begin(configStore.wifiSSID, configStore.wifiPass);
          wifi_auth_error_count++;
          //Serial.println(wifi_auth_error_count);
          if (wifi_auth_error_count > 10)
          {
            DEBUG_PRINT("wifi _ auth _ fail ");
            //reset config
            config_reset();
          }
        }
      }
    }
  }
}
//////////////////////////////////////////////////////

//##################
void requestTime_func()
{
  Blynk.sendInternal("rtc", "sync"); // used to sync rtc
}
//##################

void beep_func() {
  if (beep_var == 1)
  {
    digitalWrite(BOARD_BUZZER_PIN, HIGH);
    beep_var = 2;
  } else if (beep_var == 2) {
    digitalWrite(BOARD_BUZZER_PIN, LOW);
    beep_var = 0;
  }
}
//////////////////////////////////////////////////////
void d1_update_func() {
  Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
  if (d1_state == HIGH)
  {
    d1_led.on();
  } else
  {
    d1_led.off();
  }
}
////////////////////////////////////
void d2_update_func() {
  Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
  if (d2_state == HIGH)
  {
    d2_led.on();
  } else
  {
    d2_led.off();
  }
}
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void upd_equipment_detail_func()
{
  Blynk.virtualWrite(vir_rssi_pin_1,  WiFi.RSSI() + 120); // 120 for tolerance to make the value in positive
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case WIFI_EVENT_STAMODE_GOT_IP:
      wifi_auth_error_count = 0;
      break;
  }
}
//////////////////////////////////////////////////////////////////
void scheduler_1_func()
{
  Blynk.syncVirtual(V51); // time input widget for scheduling 1
}

void scheduler_2_func()
{
  Blynk.syncVirtual(V52); // time input widget for scheduling 2
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void arduino_serial_reader_func() {
  if (Serial.available() > 0)
  {
    arduino_serial_str = Serial.readString();
    delay(5);
  }

  if (arduino_serial_str != "") {
    arduino_serial_char_first = arduino_serial_str.charAt(0);
    if (arduino_serial_char_first == "#") {
      device_state_update_func();
    }
  }
  arduino_serial_str = "";
}
///////////////////////////////////////////////////
void device_state_update_func() {
  arduino_serial_char_rest = arduino_serial_str.substring(1, 6);
  if (arduino_serial_char_rest == "d1onX")
  {
    d1_state = HIGH;
    d1_update_func();
  }
  if (arduino_serial_char_rest == "d1off")
  {
    d1_state = LOW;
    d1_update_func();
  }
  if (arduino_serial_char_rest == "d2onX")
  {
    d2_state = HIGH;
    d2_update_func();
  }
  if (arduino_serial_char_rest == "d2off")
  {
    d2_state = LOW;
    d2_update_func();
  }
  if (arduino_serial_char_rest == "aonXX")
  {
    d1_state = HIGH;
    d2_state = HIGH;
    d1_update_func();
    d2_update_func();
  }
  if (arduino_serial_char_rest == "aoffX")
  {
    d1_state = LOW;
    d2_state = LOW;
    d1_update_func();
    d2_update_func();
  }
}


