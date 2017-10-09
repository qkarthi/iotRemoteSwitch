////////////////////////////////////////////////////////////////////////////////////
BLYNK_CONNECTED() {
  Blynk.syncAll();   // used to sync all pins when the device is connected to server
}
/////////////////////////////////////////////////////
BLYNK_WRITE(V8) // kwhr label
{
  k_watt_hr_value = param.asFloat();
}
BLYNK_WRITE(V9) // reset kwhr button
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    if (k_watt_hr_value > k_watt_hr_value_reset_maximum)
    {
      Blynk.virtualWrite(vir_kwhr_previous_pin_1, k_watt_hr_value);
      RtcDateTime current_rtc_val = Rtc.GetDateTime();
      date_string = "previous unit - reset on: " + String(current_rtc_val.Day()) + "-" + String(current_rtc_val.Month()) + "-" + String(current_rtc_val.Year());
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
  d1_update_func();
}
BLYNK_WRITE(V12) // manual button for d2
{
  d2_state  = param.asInt();
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
  }
}
///////////////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V21) // scheduler 1 activation button
{
  configStore.scheduler_1_activation_flag  = param.asInt();
  config_save();
}
BLYNK_WRITE(V22) // scheduler 2 activation button
{
  configStore.scheduler_2_activation_flag  = param.asInt();
  config_save();
}
////////////////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V51) // time input widget for scheduling 1
{
  TimeInputParam t(param);

  configStore.scheduler_1_sun_flag = false;
  configStore.scheduler_1_mon_flag = false;
  configStore.scheduler_1_tues_flag = false;
  configStore.scheduler_1_wednes_flag = false;
  configStore.scheduler_1_thurs_flag = false;
  configStore.scheduler_1_fri_flag = false;
  configStore.scheduler_1_satur_flag = false;

  if (t.isWeekdaySelected(1))
  {
    configStore.scheduler_1_mon_flag = true;
  }
  if (t.isWeekdaySelected(2))
  {
    configStore.scheduler_1_tues_flag = true;
  }
  if (t.isWeekdaySelected(3))
  {
    configStore.scheduler_1_wednes_flag = true;
  }
  if (t.isWeekdaySelected(4))
  {
    configStore.scheduler_1_thurs_flag = true;
  }
  if (t.isWeekdaySelected(5))
  {
    configStore.scheduler_1_fri_flag = true;
  }
  if (t.isWeekdaySelected(6))
  {
    configStore.scheduler_1_satur_flag = true;
  }
  if (t.isWeekdaySelected(7))
  {
    configStore.scheduler_1_sun_flag = true;
  }

  configStore.scheduler_1_on_hour = t.getStartHour();
  configStore.scheduler_1_on_minute = t.getStartMinute();
  configStore.scheduler_1_off_hour = t.getStopHour();
  configStore.scheduler_1_off_minute = t.getStopMinute();
  config_save();
}
BLYNK_WRITE(V52) // time input widget for scheduling 2
{
  TimeInputParam t(param);

  configStore.scheduler_2_sun_flag = false;
  configStore.scheduler_2_mon_flag = false;
  configStore.scheduler_2_tues_flag = false;
  configStore.scheduler_2_wednes_flag = false;
  configStore.scheduler_2_thurs_flag = false;
  configStore.scheduler_2_fri_flag = false;
  configStore.scheduler_2_satur_flag = false;

  if (t.isWeekdaySelected(1))
  {
    configStore.scheduler_2_mon_flag = true;
  }
  if (t.isWeekdaySelected(2))
  {
    configStore.scheduler_2_tues_flag = true;
  }
  if (t.isWeekdaySelected(3))
  {
    configStore.scheduler_2_wednes_flag = true;
  }
  if (t.isWeekdaySelected(4))
  {
    configStore.scheduler_2_thurs_flag = true;
  }
  if (t.isWeekdaySelected(5))
  {
    configStore.scheduler_2_fri_flag = true;
  }
  if (t.isWeekdaySelected(6))
  {
    configStore.scheduler_2_satur_flag = true;
  }
  if (t.isWeekdaySelected(7))
  {
    configStore.scheduler_2_sun_flag = true;
  }

  configStore.scheduler_2_on_hour = t.getStartHour();
  configStore.scheduler_2_on_minute = t.getStartMinute();
  configStore.scheduler_2_off_hour = t.getStopHour();
  configStore.scheduler_2_off_minute = t.getStopMinute();
  config_save();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V71) // scheduler 1 device selecter
{
  configStore.scheduler_1_selected_device = param.asInt();
  config_save();
}
BLYNK_WRITE(V72) // scheduler 2 device selecter
{
  configStore.scheduler_2_selected_device = param.asInt();
  config_save();
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
    RtcDateTime current_rtc_val = Rtc.GetDateTime();
    term_time_string = String(current_rtc_val.Hour()) + ":" + String(current_rtc_val.Minute()) + ":" + String(current_rtc_val.Second());
    terminal.println(term_time_string);
  }   else if ((String("d") == term_char_one) & (String("ate") == term_char_rest)) {
    RtcDateTime current_rtc_val = Rtc.GetDateTime();
    term_time_string = String(current_rtc_val.Day()) + "/" + String(current_rtc_val.Month()) + "/" + String(current_rtc_val.Year()) + " - week day = " + String(current_rtc_val.DayOfWeek() + 1);
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
  } else  if ((String("*") == term_char_one)) {
    Blynk.virtualWrite(vir_equipment_name_pin_1, term_char_rest);
    Blynk.virtualWrite(vir_equipment_name_pin_2, term_char_rest);
    CopyString(term_char_rest, configStore.equip_name);
    config_save();
    Blynk.virtualWrite(99,  configStore.equip_name);
    terminal.println("success");
  }  else if (String("1") == term_char_one) {
    Blynk.virtualWrite(vir_d1_label_pin_1, term_char_rest);
    CopyString(term_char_rest, configStore.dev_1_name);
    config_save();
    Blynk.setProperty(vir_d1_man_button_pin_1, "label", term_char_rest);
    Blynk.setProperty(vir_device_selector_1, "labels", configStore.dev_1_name, configStore.dev_2_name);
    Blynk.setProperty(vir_device_selector_2, "labels", configStore.dev_1_name, configStore.dev_2_name);
    terminal.println("success");
  } else if (String("2") == term_char_one) {
    Blynk.virtualWrite(vir_d2_label_pin_1, term_char_rest);
    CopyString(term_char_rest, configStore.dev_2_name);
    config_save();
    Blynk.setProperty(vir_d2_man_button_pin_1, "label", term_char_rest);
    Blynk.setProperty(vir_device_selector_1, "labels", configStore.dev_1_name, configStore.dev_2_name);
    Blynk.setProperty(vir_device_selector_2, "labels", configStore.dev_1_name, configStore.dev_2_name);
    terminal.println("success");
  } else if (String("@") == term_char_one) {
    Serial.print( configStore.scheduler_1_sun_flag); Serial.print( configStore.scheduler_1_mon_flag); Serial.print( configStore.scheduler_1_tues_flag); Serial.print( configStore.scheduler_1_wednes_flag); Serial.print( configStore.scheduler_1_thurs_flag); Serial.print( configStore.scheduler_1_fri_flag); Serial.print( configStore.scheduler_1_satur_flag);
    Serial.println("");
    Serial.println(configStore.scheduler_1_on_hour); Serial.println(configStore.scheduler_1_on_minute); Serial.println(configStore.scheduler_1_off_hour); Serial.println(configStore.scheduler_1_off_minute);
  }
  terminal.flush();
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
void con_manager_func()
{
  //DEBUG_PRINT("ver_2");
  if (WiFi.status() == WL_CONNECTED)
  {
    state_wifi = true;
    DEBUG_PRINT("wifi _ true ");
    if ((Blynk.connected()) == false)
    {
      state_cloud = false;
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
      state_cloud = true;
      DEBUG_PRINT("cloud _ true ");
    }
  }
  else
  {

    digitalWrite(BOARD_BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BOARD_BUZZER_PIN, LOW);

    //beep once for 10 sec
    state_wifi = false;
    DEBUG_PRINT("wifi _ false ");
    if (BlynkState::is(MODE_RUNNING)||BlynkState::is(MODE_CONNECTING_CLOUD)) {
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
  digitalWrite(d1_op_pin_1, d1_state);
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
  digitalWrite(d2_op_pin_1, d2_state);
  if (d2_state == HIGH)
  {
    d2_led.on();
  } else
  {
    d2_led.off();
  }
}
/////////////////////////////////////////////////////////////////////////
void upd_d1_hw_switch_1_func()
{
  DEBUG_PRINT("hw_switch_1");
  if (digitalRead(d1_hw_switch_pin_1) != d1_hw_switch_1_state)
  {
    d1_hw_switch_1_state = !d1_hw_switch_1_state;
    d1_state = !d1_state;
    Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
    d1_update_func();
  }
}
///////////////////////////////
void upd_d2_hw_switch_1_func()
{
  DEBUG_PRINT("hw_switch_2");
  if (digitalRead(d2_hw_switch_pin_1) != d2_hw_switch_1_state)
  {
    d2_hw_switch_1_state = !d2_hw_switch_1_state;
    d2_state = !d2_state;
    Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
    d2_update_func();
  }
}
//////////////////////////////////////////////////////////////////////
void upd_equipment_detail_func()
{
  Blynk.virtualWrite(vir_rssi_pin_1,  WiFi.RSSI() + 120); // 120 for tolerance to make the value in positive
}
//////////////////////////////////////////////////////////////////////
void amp_mes_func()
{
  int readValue;
  int maxValue = 0;
  int minValue = 1024;
  uint32_t start_time = millis();
  while ((millis() - start_time) < 200) //sample for 1 Sec
  {
    readValue = analogRead(sensorIn);
    if (readValue > maxValue)
    {
      maxValue = readValue;
    }
    if (readValue < minValue)
    {
      minValue = readValue;
    }
  }
  sensor_result = ((maxValue - minValue) * 5.0) / 1024.0; //converting to 5 v dc.
  VRMS = (sensor_result / 2.0) * 0.707; // root means square value
  AmpsRMS = (((VRMS * 1000) / mVperAmp) - 0.26);
  if ((d2_state == HIGH) | (d1_state == HIGH)) // checks if the device working or not for current sensor measurement
  {
    if (AmpsRMS < 0.1) // for filter clearence
    {
      AmpsRMS = 0;
    }
    Blynk.virtualWrite(vir_sensor_pin_1, AmpsRMS);// sending values to virtual pin (sensor pin)
    Blynk.virtualWrite(vir_amp_pin_1, AmpsRMS);// sending values to virtual pin (amps)
    Blynk.virtualWrite(vir_watts_pin_1, AmpsRMS * 240);// sending values to virtual pin with multiples of 240 ( watts)
    float watts_val = (AmpsRMS * 240);
    float k_watts_val = (watts_val / 1000);
    float k_watts_hr_val = (watts_val * 0.000277778 * 3);
    k_watt_hr_value = k_watts_hr_val + k_watt_hr_value;
    Blynk.virtualWrite(vir_kwhr_pin_1, k_watt_hr_value);
  }
}
//////////////////////////////////////////////////////////////////
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case WIFI_EVENT_STAMODE_GOT_IP:
      wifi_auth_error_count = 0;
      break;
  }
}
//////////////////////////////////////////////////////////////////
void scheduler_1_validator_func(int operation)
{
  RtcDateTime current_rtc_val = Rtc.GetDateTime();
  switch (current_rtc_val.DayOfWeek()) {
    case 1:
      if (configStore.scheduler_1_mon_flag == true)
      {
        scheduler_1_executer_func(operation);
      }
      break;
    case 2:
      if (configStore.scheduler_1_tues_flag == true)
      {
        scheduler_1_executer_func(operation);
      }
      break;
    case 3:
      if (configStore.scheduler_1_wednes_flag == true)
      {
        scheduler_1_executer_func(operation);
      }
      break;
    case 4:
      if (configStore.scheduler_1_thurs_flag == true)
      {
        scheduler_1_executer_func(operation);
      }
      break;
    case 5:
      if (configStore.scheduler_1_fri_flag == true)
      {
        scheduler_1_executer_func(operation);
      }
      break;
    case 6:
      if (configStore.scheduler_1_satur_flag == true)
      {
        scheduler_1_executer_func(operation);
      }
      break;
    case 7:
      if (configStore.scheduler_1_sun_flag == true)
      {
        scheduler_1_executer_func(operation);
      }
      break;
    default:
      break;
  }
}



void scheduler_1_func() {
  DEBUG_PRINT("scheduler_1");
  if (configStore.scheduler_1_activation_flag == true)
  {
    RtcDateTime current_rtc_val = Rtc.GetDateTime();
    if ((configStore.scheduler_1_on_hour == current_rtc_val.Hour()) & (configStore.scheduler_1_on_minute == current_rtc_val.Minute()))
    {
      scheduler_1_validator_func(1);
    }
    else if ((configStore.scheduler_1_off_hour == current_rtc_val.Hour()) & (configStore.scheduler_1_off_minute == current_rtc_val.Minute()))
    {
      scheduler_1_validator_func(0);
    }
  }
}


void scheduler_1_executer_func(int state_of_operation)
{
  int selected_device_var = configStore.scheduler_1_selected_device;
  if  (state_of_operation)
  {
    scheduler_str = "scheduling Started";
  }
  else {
    scheduler_str = "scheduling Finished";
  }
  if (selected_device_var == 1)
  {
    d1_state = state_of_operation;
    d1_update_func(); //send values to devices for update
    Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
    Blynk.notify(String(configStore.equip_name) + " - " + String(configStore.dev_1_name) + " - " + scheduler_str);

  } else if (selected_device_var == 2)
  {
    d2_state = state_of_operation;
    d2_update_func(); //send values to devices for update
    Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
    Blynk.notify(String(configStore.equip_name) + " - " + String(configStore.dev_2_name) + " - " + scheduler_str);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void scheduler_2_validator_func(int operation)
{
  RtcDateTime current_rtc_val = Rtc.GetDateTime();
  switch (current_rtc_val.DayOfWeek()) {
    case 1:
      if (configStore.scheduler_2_mon_flag == true)
      {
        scheduler_2_executer_func(operation);
      }
      break;
    case 2:
      if (configStore.scheduler_2_tues_flag == true)
      {
        scheduler_2_executer_func(operation);
      }
      break;
    case 3:
      if (configStore.scheduler_2_wednes_flag == true)
      {
        scheduler_2_executer_func(operation);
      }
      break;
    case 4:
      if (configStore.scheduler_2_thurs_flag == true)
      {
        scheduler_2_executer_func(operation);
      }
      break;
    case 5:
      if (configStore.scheduler_2_fri_flag == true)
      {
        scheduler_2_executer_func(operation);
      }
      break;
    case 6:
      if (configStore.scheduler_2_satur_flag == true)
      {
        scheduler_2_executer_func(operation);
      }
      break;
    case 7:
      if (configStore.scheduler_2_sun_flag == true)
      {
        scheduler_2_executer_func(operation);
      }
      break;
    default:
      break;
  }
}



void scheduler_2_func() {
  DEBUG_PRINT("scheduler_2");
  if (configStore.scheduler_2_activation_flag == true)
  {
    RtcDateTime current_rtc_val = Rtc.GetDateTime();
    if ((configStore.scheduler_2_on_hour == current_rtc_val.Hour()) & (configStore.scheduler_2_on_minute == current_rtc_val.Minute()) )
    {
      scheduler_2_validator_func(1);
    }
    else if ((configStore.scheduler_2_off_hour == current_rtc_val.Hour()) & (configStore.scheduler_2_off_minute == current_rtc_val.Minute()))
    {
      scheduler_2_validator_func(0);
    }
  }
}


void scheduler_2_executer_func(int state_of_operation)
{
  int selected_device_var = configStore.scheduler_2_selected_device;
  if  (state_of_operation)
  {
    scheduler_str = "scheduling Started";

  }
  else {
    scheduler_str = "scheduling Finished";
  }
  if (selected_device_var == 1)
  {
    d1_state = state_of_operation;
    d1_update_func(); //send values to devices for update
    Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
    Blynk.notify(String(configStore.equip_name) + " - " + String(configStore.dev_1_name) + " - " + scheduler_str);

  }
  else if (selected_device_var == 2)
  {
    d2_state = state_of_operation;
    d2_update_func(); //send values to devices for update
    Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
    Blynk.notify(String(configStore.equip_name) + " - " + String(configStore.dev_2_name) + " - " + scheduler_str);
  }
}

/////////////////////////////////////////////////////////////////////////////////

void connectivity_fall_back_func(){
  WiFi.disconnect();
  DEBUG_PRINT(String("Connecting to WiFi: ") + configStore.wifiSSID);
  WiFi.begin(configStore.wifiSSID, configStore.wifiPass);
  if (WiFi.status() == WL_CONNECTED) {
     DEBUG_PRINT(String(" wifi ready"));
    state_wifi=true;
  Blynk.disconnect();
  Blynk.config(configStore.cloudToken, configStore.cloudHost, configStore.cloudPort);
  Blynk.connect(0);
  if ((Blynk.connected()) == true)
    { 
      DEBUG_PRINT(String(" cloud ready"));
       state_cloud==true;
}
}
}




