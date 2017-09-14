#define USE_CUSTOM_BOARD
#define APP_DEBUG
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include "BlynkProvisioning.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
//////////////////////////////////////////////////////
BlynkTimer  con_manager, acs_timer, rtc_time_req_timer, upd_equipment_detail_timer;
BlynkTimer  d1_hw_switch_timer, d2_hw_switch_timer;
BlynkTimer  scheduler_1_timer, scheduler_2_timer;

WidgetTerminal terminal(V99);
WidgetLED d1_led(V61), d2_led(V62);
WidgetRTC rtc;
//////////////////////////////////////////////////////
const int sensorIn = A0;

//const int d1_hw_switch_pin_1 = 0; // nodemcu
const int d1_hw_switch_pin_1 = 12; //lab
const int d2_hw_switch_pin_1 = 13;

const int d1_op_pin_1 = 16;
const int d2_op_pin_1 = 14;
////////////////////////////////////////////////////////
const int vir_equipment_name_pin_1 = 0;

const int vir_equipment_ip_address_pin_1 = 1;

const int vir_rssi_pin_1 = 2;

const int vir_sensor_pin_1 = 3;

const int vir_amp_pin_1 = 4;

//const int vir_terminal_pin = 99;
//const int vir_internal_ota_button_pin = 100;
//const int vir_ota_button_pin = 111;
//////////////////////////////////////////
const int vir_watts_pin_1 = 5;
const int vir_kwhr_pin_1 = 8;
///////////////////////////////////////////
const int vir_master_power_off_man_button_pin_1 = 20;
const int vir_d1_man_button_pin_1 = 11;
const int vir_d2_man_button_pin_1 = 12;
///////////////////////////////////////////
const int vir_scheduler_1_button_pin_1 = 21;
const int vir_scheduler_2_button_pin_1 = 22;
//////////////////////////////////////////
const int vir_d1_label_pin_1 = 31;
const int vir_d2_label_pin_1 = 32;
///////////////////////////////////////////
const int vir_scheduler_1_time_input_pin_1 = 51;
const int vir_scheduler_2_time_input_pin_1 = 52;
//////////////////////////////////////////
//const int vir_d1_led_pin_1 = 61;
//const int vir_d2_led_pin_1 = 62;
///////////////////////////////////////////
const int vir_device_selector_1 = 71;
const int vir_device_selector_2 = 72;

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

int wifi_auth_error_count = 0;
///////////////////////////////////////////
int scheduler_1_var_1;
int scheduler_2_var_1;

int device_selected_scheduler_1;
int device_selected_scheduler_2;

long additional_scheduling_intervals = 20;

long scheduler_1_start_seconds;
long scheduler_2_start_seconds;

long scheduler_1_stop_seconds;
long scheduler_2_stop_seconds;
///////////////////////////////////////////
long nowseconds_var;
int dayadjustment_var;
int day_i;
/***********************************/
int mVperAmp = 75;
double sensor_result;
double VRMS = 0;
double AmpsRMS = 0;

/////////////////////////////////////////////
String term_string, term_char_one, term_char_rest, term_time_string;
/////////////////////////////////////////////
int d1_state = LOW;
int d1_hw_switch_1_state = HIGH;

int d2_state = LOW;
int d2_hw_switch_1_state = HIGH;
///////////////////////////////////////////////////////
float k_watt_hr_value;
//////////////////////////////////////////////////////

void d1_update() {
  digitalWrite(d1_op_pin_1, d1_state);
  if (d1_state == HIGH)
  {
    d1_led.on();
  } else
  {
    d1_led.off();
  }
}


void d2_update() {
  digitalWrite(d2_op_pin_1, d2_state);
  if (d2_state == HIGH)
  {
    d2_led.on();
  } else
  {
    d2_led.off();
  }
}

//////////////////////////////////////////////////////
void requestTime_func() {
  Blynk.sendInternal("rtc", "sync"); // used to sync rtc
  DEBUG_PRINT("sync");
}

BLYNK_CONNECTED() {
  Blynk.syncAll();   // used to sync all pins when the device is connected to server
}
//////////////////////////////////////////////////////
BLYNK_WRITE(V11)///////////////////
{
  d1_state  = param.asInt();
  d1_update();
}
BLYNK_WRITE(V8)
{
  k_watt_hr_value = param.asFloat();
}
BLYNK_WRITE(V12)///////////////////
{
  d2_state  = param.asInt();
  d2_update();
}
BLYNK_WRITE(V20)///////////////////
{
  int pinValue = param.asInt();
  if (pinValue == 1)
  {
    d1_state  = LOW;
    d2_state  = LOW;
    Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
    Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
    d1_update();
    d2_update();
  }
}
////////////////////////////////////////////////////////
void con_manager_func()
{
  //DEBUG_PRINT("ver_2");
  if (WiFi.status() == WL_CONNECTED)
  {
    // DEBUG_PRINT("wifi _ true ");
    if ((Blynk.connected()) == false)
    {
      // DEBUG_PRINT("blynk _ false ");
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
    // DEBUG_PRINT("wifi _ false ");
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



////////////////////////////////////////////////////////////////////////////////////////////////////////


BLYNK_WRITE(V51)
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
            if (device_selected_scheduler_1 = 1)
            {
              d1_state = HIGH;
              d1_update(); //send values to devices for update
              Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
              Blynk.notify(String(configStore.mspace_1) + " powered on ");
            }
            if (device_selected_scheduler_1 = 2)
            {
              d2_state = HIGH;
              d2_update(); //send values to devices for update
              Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
              Blynk.notify(String(configStore.mspace_2) + " powered on ");
            }
          }
          /////////////////////////////////////////
          if ( ((scheduler_1_stop_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_1_stop_seconds))
          { if (device_selected_scheduler_1 = 1)
            {
              d1_state = LOW;
              d1_update(); //send values to devices for update
              Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);

              Blynk.notify(String(configStore.mspace_1) + " powered off ");
            }
            if (device_selected_scheduler_1 = 2)
            {
              d2_state = LOW;
              d2_update(); //send values to devices for update
              Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
              Blynk.notify(String(configStore.mspace_2) + " powered off ");
            }
          }
        }
      }
    }
  }
}

BLYNK_WRITE(V52)
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
          { if (device_selected_scheduler_2 = 1)
            {
              d1_state = HIGH;
              d1_update(); //send values to devices for update
              Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);

              Blynk.notify(String(configStore.mspace_1) + " powered on ");
            } if (device_selected_scheduler_2 = 2)
            {
              d2_state = HIGH;
              d2_update(); //send values to devices for update
              Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
              Blynk.notify(String(configStore.mspace_2) + " powered on ");
            }
          }
          /////////////////////////////////////////
          if ( ((scheduler_2_stop_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_2_stop_seconds))
          { if (device_selected_scheduler_2 = 1)
            {
              d1_state = LOW;
              d1_update(); //send values to devices for update
              Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
              Blynk.notify(String(configStore.mspace_1) + " powered off ");
            } if (device_selected_scheduler_2 = 2)
            {
              d2_state = LOW;
              d2_update(); //send values to devices for update
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

BLYNK_WRITE(V71)
{
  device_selected_scheduler_1 = param.asInt();
}

BLYNK_WRITE(V72)
{
  device_selected_scheduler_2 = param.asInt();
}
//////////////////////////////////////////////////////////////////////

void scheduler_1_func()
{
  Blynk.syncVirtual(V51);
}

void scheduler_2_func()
{
  Blynk.syncVirtual(V52);
}

////////////////////////////////////////////////////////////////////

BLYNK_WRITE(V21)///////////////////
{
  scheduler_1_var_1  = param.asInt();
}
BLYNK_WRITE(V22)///////////////////
{
  scheduler_2_var_1  = param.asInt();
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




void upd_d1_hw_switch_1_func()
{
  if (digitalRead(d1_hw_switch_pin_1) != d1_hw_switch_1_state)
  {
    d1_hw_switch_1_state = !d1_hw_switch_1_state;
    d1_state = !d1_state;
    Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
    d1_update();
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
    d2_update();
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
    float watts_val = (AmpsRMS * 240);
    float k_watts_val = (watts_val / 1000);
    float k_watts_hr_val = (watts_val * 0.000277778 * 3);
    k_watt_hr_value = k_watts_hr_val + k_watt_hr_value;
    Blynk.virtualWrite(vir_kwhr_pin_1, k_watt_hr_value); //
  }
}


void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case WIFI_EVENT_STAMODE_GOT_IP:
      wifi_auth_error_count = 0;
      break;
  }
}

void setup() {
  Serial.begin(115200); // declering the serial monitor with baudrate

  BlynkProvisioning.begin(); //blynk provision

  WiFi.onEvent(WiFiEvent);
  con_manager.setInterval(10000L, con_manager_func);

  rtc_time_req_timer.setInterval(10000L, requestTime_func); // setting the rtc timer with regular interval

  upd_equipment_detail_timer.setInterval(5000, upd_equipment_detail_func); // setting the equipment detail update timer with regular interval

  acs_timer.setInterval(3000, amp_mes_func); // setting the current sensor timer with regular interval

  d1_hw_switch_timer.setInterval(500, upd_d1_hw_switch_1_func);// setting the hardware switch timer with regular interval
  d2_hw_switch_timer.setInterval(500, upd_d2_hw_switch_1_func);// setting the hardware switch timer with regular interval

  scheduler_1_timer.setInterval(10000, scheduler_1_func); // setting the device 1 timer with regular interval
  scheduler_2_timer.setInterval(10000, scheduler_2_func); // setting the device 2 timer with regular interval

  pinMode(d1_hw_switch_pin_1, INPUT_PULLUP); // declaring the pin as input-pullup
  pinMode(d2_hw_switch_pin_1, INPUT_PULLUP); // declaring the pin as input-pullup

  pinMode(d1_op_pin_1, OUTPUT); // declaring the pin as output
  pinMode(d2_op_pin_1, OUTPUT); // declaring the pin as output



  d1_hw_switch_1_state = digitalRead(d1_hw_switch_pin_1); // getting the state of hardware device 1 switch
  d2_hw_switch_1_state = digitalRead(d2_hw_switch_pin_1); // getting the state of hardware device 2 switch
}

void loop()
{
  BlynkProvisioning.run(); //used to run provision
  con_manager.run();

  acs_timer.run();  // used to run current sensor time
  rtc_time_req_timer.run();  //  used to run real time clock
  upd_equipment_detail_timer.run();  // used to update the wifi and device name

  d1_hw_switch_timer.run(); scheduler_1_timer.run();  //device 1's hardware switch timer and schedule timer
  d2_hw_switch_timer.run(); scheduler_2_timer.run();  //device 2's hardware switch timer and schedule timer
}