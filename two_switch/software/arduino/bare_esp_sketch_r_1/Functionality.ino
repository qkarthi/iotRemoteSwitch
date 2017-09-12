///////////////////////////////////////////
BLYNK_CONNECTED() {
  Blynk.syncAll();   // used to sync all pins when the device is connected to server
}
///////////////////////////////////////////
void con_manager_func()
{
   DEBUG_PRINT("ver_2");
  if (WiFi.status() == WL_CONNECTED)
  {
    DEBUG_PRINT("wifi _ true ");

    if ((Blynk.connected()) == false)
    {
      DEBUG_PRINT("blynk _ false ");
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
     // DEBUG_PRINT("blynk _ true ");
    }
  }
  else
  {
    digitalWrite(BOARD_BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BOARD_BUZZER_PIN, LOW);
    //beep once for 10 sec
    DEBUG_PRINT("wifi _ false ");
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
//////////////////////////////////////////
void requestTime_func() {
  Blynk.sendInternal("rtc", "sync"); // used to sync rtc
}
//////////////////////////////////////////
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
/////////////////////////////////////////////////////
BLYNK_WRITE(V11)//manual button for d1
{
  d1_state  = param.asInt();
  d1_update_func();
}
BLYNK_WRITE(V12)//manual button for d2
{
  d2_state  = param.asInt();
  d2_update_func();
}
BLYNK_WRITE(V20)//master button for power off all switch
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
//////////////////////////////////////////////////////////
BLYNK_WRITE(V8)// kwhr label
{
  k_watt_hr_value = param.asFloat();
}
////////////////////////////////////////////////////////
BLYNK_WRITE(V51)
{
  if (d1_scheduler_var_1 == HIGH) //scd_button selected
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
          d1_startseconds = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);
          d1_stopseconds = (t.getStopHour() * 3600) + (t.getStopMinute() * 60) + additional_scheduling_intervals;
          if (((d1_startseconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= d1_startseconds))
          {
            if (d1_state != HIGH)
            {
              d1_state = HIGH;
              d1_update_func(); //send values to devices for update
            }
            Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
          }
          if ( ((d1_stopseconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= d1_stopseconds))
          {
            if (d1_state != LOW)
            {
              d1_state = LOW;
              d1_update_func();  //send values to devices for update
            }
            Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
          }
        }
      }
    }
  }
}
/******************************************************************************/
BLYNK_WRITE(V52)
{
  if (d2_scheduler_var_1 == HIGH) //scd_button selected
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
          d2_startseconds = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);
          d2_stopseconds = (t.getStopHour() * 3600) + (t.getStopMinute() * 60) + additional_scheduling_intervals;
          if (((d2_startseconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= d2_startseconds))
          {
            d2_state = HIGH;
            d2_update_func(); //send values to devices for update
            Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
          }
          if ( ((d2_stopseconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= d2_stopseconds))
          {
            d2_state = LOW;
            d2_update_func(); //send values to devices for update
            Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
          }
        }
      }
    }
  }
}


//////////////////////////////////////////////////////////////////////
void d1_scheduled_func() // time input widget for scheduling 1
{
  Blynk.syncVirtual(V51);
}

void d2_scheduled_func() // time input widget for scheduling 2
{
  Blynk.syncVirtual(V52);
}

////////////////////////////////////////////////////////////////////

BLYNK_WRITE(V21) // scheduler 1 button
{
  d1_scheduler_var_1  = param.asInt();
}
BLYNK_WRITE(V22) // scheduler 2  button
{
  d2_scheduler_var_1  = param.asInt();
}

////////////////////////////////////////////////////////////////////


BLYNK_WRITE(V99)/////////////////////
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
    terminal.println(" ");
    term_time_string = String(hour()) + ":" + String(minute()) + ":" + String(second());
    terminal.println(term_time_string);
  } else  if ((String("*") == term_char_one)) {
    Blynk.virtualWrite(vir_equipment_name_pin_1, term_char_rest);
    CopyString(term_char_rest, configStore.equip_name);
    config_save();
    // run_ota_func();
    Blynk.virtualWrite(99,  configStore.equip_name);
    terminal.println("success");
  }  else if (String("1") == term_char_one) {
    Blynk.virtualWrite(vir_d1_label_pin_1, term_char_rest);
    Blynk.virtualWrite(vir_d1_label_pin_2, term_char_rest);
    Blynk.virtualWrite(vir_d1_label_pin_3, term_char_rest);
    Blynk.setProperty(vir_d1_man_button_pin_1, "label", term_char_rest);
    terminal.println("success");
  } else if (String("2") == term_char_one) {
    Blynk.virtualWrite(vir_d2_label_pin_1, term_char_rest);
    Blynk.virtualWrite(vir_d2_label_pin_2, term_char_rest);
    Blynk.virtualWrite(vir_d2_label_pin_3, term_char_rest);
    Blynk.setProperty(vir_d2_man_button_pin_1, "label", term_char_rest);
    terminal.println("success");
  }
  terminal.flush();
}




void upd_d1_hw_switch_1_func()
{
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
  if (digitalRead(d2_hw_switch_pin_1) != d2_hw_switch_1_state)
  {
    d2_hw_switch_1_state = !d2_hw_switch_1_state;
    d2_state = !d2_state;
    Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
    d2_update_func();
  }
}
void upd_equipment_detail_func()
{
  Blynk.virtualWrite(vir_rssi_pin_1,  WiFi.RSSI() + 120); // 120 for tolerance to make the value in positive

}


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
  if ((d2_state == HIGH) | (d1_state = HIGH)) // checks if the device working or not for current sensor measurement
  {
    if (AmpsRMS < 0.1) // for filter clearence
    {
      AmpsRMS = 0;
    }





    Blynk.virtualWrite(vir_sensor_pin_1, AmpsRMS);// sending values to virtual pin (sensor pin)
    Blynk.virtualWrite(vir_amp_pin_1, AmpsRMS);// sending values to virtual pin (amps)
    Blynk.virtualWrite(vir_watts_pin_1, AmpsRMS * 240);// sending values to virtual pin with multiples of 240 ( watts)
    Blynk.virtualWrite(vir_watts_pin_2, AmpsRMS * 240);// sending values to virtual pin with multiples of 240 ( watts)
    Blynk.virtualWrite(vir_watts_pin_3, AmpsRMS * 240);// sending values to virtual pin with multiples of 240 ( watts)
    float watts_val = (AmpsRMS * 240);
    float k_watts_val = (watts_val / 1000);
    float k_watts_hr_val = (watts_val * 0.000277778 * 3);
    k_watt_hr_value = k_watts_hr_val + k_watt_hr_value;
    Blynk.virtualWrite(vir_kwhr_pin_1, k_watt_hr_value); //
  }
}

//void run_ota_func() {
//  ArduinoOTA.setHostname(configStore.equip_name); // getting the value in eeprom
//  ArduinoOTA.setPort(8266); // port for host and file upload
//  ArduinoOTA.setPassword((const char *)"fslabfslab"); // fslabfslab is password
//  ArduinoOTA.begin();
//}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case WIFI_EVENT_STAMODE_GOT_IP:
      wifi_auth_error_count = 0;
      break;
  }
}
