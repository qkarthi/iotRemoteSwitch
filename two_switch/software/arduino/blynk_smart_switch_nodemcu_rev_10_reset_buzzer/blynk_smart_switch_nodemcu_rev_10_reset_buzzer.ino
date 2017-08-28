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
BlynkTimer acs_timer, rtc_time_req_timer, upd_equipment_detail_timer, con_manager;
BlynkTimer  d1_hw_switch_timer, d1_scheduled_timer, d2_hw_switch_timer, d2_scheduled_timer;
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
//////////////////////////////////////////
const int vir_watts_pin_1 = 5;
const int vir_watts_pin_2 = 6;
const int vir_watts_pin_3 = 7;

const int vir_kwhr_pin_1 = 8;
///////////////////////////////////////////
const int vir_master_power_off_man_button_pin_1 = 20;
const int vir_d1_man_button_pin_1 = 11;
const int vir_d2_man_button_pin_1 = 12;
///////////////////////////////////////////
const int vir_d1_scheduler_button_pin_1 = 21;
const int vir_d2_scheduler_button_pin_1 = 22;
//////////////////////////////////////////
const int vir_d1_label_pin_1 = 31;
const int vir_d2_label_pin_1 = 32;

const int vir_d1_label_pin_2 = 35;
const int vir_d2_label_pin_2 = 36;

const int vir_d1_label_pin_3 = 39;
const int vir_d2_label_pin_3 = 40;
///////////////////////////////////////////
const int vir_d1_time_input_pin_1 = 51;
const int vir_d2_time_input_pin_1 = 52;
//////////////////////////////////////////
//const int vir_d1_led_pin_1 = 61;
//const int vir_d2_led_pin_1 = 62;
//const int vir_d3_led_pin_1 = 63;
//const int vir_d4_led_pin_1 = 64;
///////////////////////////////////////////
long additional_scheduling_intervals = 20;

long d1_startseconds;
long d2_startseconds;

long d1_stopseconds;
long d2_stopseconds;
///////////////////////////////////////////
long nowseconds_var;
int dayadjustment_var;
int day_i;
/***********************************/
int mVperAmp = 75;
double sensor_result;
double VRMS = 0;
double AmpsRMS = 0;

String term_string, term_char_one, term_char_rest, term_time_string;

int d1_state = LOW;
int d1_scheduler_var_1;
int d1_hw_switch_1_state = HIGH;

int d2_state = LOW;
int d2_scheduler_var_1;
int d2_hw_switch_1_state = HIGH;

///////////////////////////////////////////////////////
long d1_hw_switch_1_previous_millis = 0;
long d2_hw_switch_1_previous_millis = 0;

int d1_hw_switch_1_reset_count = 0;
int d2_hw_switch_1_reset_count = 0;
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
    //Blynk.virtualWrite(vir_master_power_off_man_button_pin_1, 0); // uncommnet for debug purpose
  }
}
////////////////////////////////////////////////////////
void con_manager_func()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if ((Blynk.connected()) == false)
    {
      digitalWrite(BOARD_BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BOARD_BUZZER_PIN, LOW);
      delay(500);
      digitalWrite(BOARD_BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BOARD_BUZZER_PIN, LOW);
      //beep twice for 10 sec
    }
  }
  else
  {
    // lets scan for available ssid
    int n = WiFi.scanNetworks();
    //if ssid found
    if (n > 0)
    {
      for (int i = 0; i < n; ++i)
      { // check weather the ssid match with saved ssid
        if (WiFi.SSID(i) == configStore.wifiSSID)
        {
          //reset config
          config_reset();
        }
        else {
          digitalWrite(BOARD_BUZZER_PIN, HIGH);
          delay(500);
          digitalWrite(BOARD_BUZZER_PIN, LOW);
          //beep once for 10 sec
        }
      }

    }
    else
    {
      digitalWrite(BOARD_BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BOARD_BUZZER_PIN, LOW);
      //beep once for 10 sec
    }
  }
}



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
              d1_update(); //send values to devices for update
            }
            Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
          }
          if ( ((d1_stopseconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= d1_stopseconds))
          {
            if (d1_state != LOW)
            {
              d1_state = LOW;
              d1_update();  //send values to devices for update
            }
            Blynk.virtualWrite(vir_d1_man_button_pin_1, d1_state);
          }
        }
      }
    }
  }
}







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
            d2_update(); //send values to devices for update
            Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
          }
          if ( ((d2_stopseconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= d2_stopseconds))
          {
            d2_state = LOW;
            d2_update(); //send values to devices for update
            Blynk.virtualWrite(vir_d2_man_button_pin_1, d2_state);
          }
        }
      }
    }
  }
}


//////////////////////////////////////////////////////////////////////
void d1_scheduled_func()
{
  Blynk.syncVirtual(V51);
}

void d2_scheduled_func()
{
  Blynk.syncVirtual(V52);
}

////////////////////////////////////////////////////////////////////

BLYNK_WRITE(V21)///////////////////
{
  d1_scheduler_var_1  = param.asInt();
}
BLYNK_WRITE(V22)///////////////////
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



void setup() {
  Serial.begin(115200); // declering the serial monitor with baudrate

  BlynkProvisioning.begin(); //blynk provision

  con_manager.setInterval(10000L, con_manager_func);

  rtc_time_req_timer.setInterval(60000L, requestTime_func); // setting the rtc timer with regular interval

  upd_equipment_detail_timer.setInterval(5000, upd_equipment_detail_func); // setting the equipment detail update timer with regular interval

  acs_timer.setInterval(3000, amp_mes_func); // setting the current sensor timer with regular interval

  d1_hw_switch_timer.setInterval(500, upd_d1_hw_switch_1_func);// setting the hardware switch timer with regular interval
  d2_hw_switch_timer.setInterval(500, upd_d2_hw_switch_1_func);// setting the hardware switch timer with regular interval

  d1_scheduled_timer.setInterval(10000, d1_scheduled_func); // setting the device 1 timer with regular interval
  d2_scheduled_timer.setInterval(10000, d2_scheduled_func); // setting the device 2 timer with regular interval

  pinMode(d1_hw_switch_pin_1, INPUT_PULLUP); // declaring the pin as input-pullup
  pinMode(d2_hw_switch_pin_1, INPUT_PULLUP); // declaring the pin as input-pullup

  pinMode(d1_op_pin_1, OUTPUT); // declaring the pin as output
  pinMode(d2_op_pin_1, OUTPUT); // declaring the pin as output

  d1_hw_switch_1_state = digitalRead(d1_hw_switch_pin_1); // getting the state of hardware device 1 switch
  d2_hw_switch_1_state = digitalRead(d2_hw_switch_pin_1); // getting the state of hardware device 2 switch
  // run_ota_func();   //uncomment ota function arduino  method
}

void loop() {
  //ArduinoOTA.handle();  // uncomment to run the ota function arduino method
  BlynkProvisioning.run(); //used to run provision

  acs_timer.run();  // used to run current sensor time
  rtc_time_req_timer.run();  //  used to run real time clock
  upd_equipment_detail_timer.run();  // used to update the wifi and device name


  con_manager.run();
  d1_hw_switch_timer.run(); d1_scheduled_timer.run();  //device 1's hardware switch timer and schedule timer
  d2_hw_switch_timer.run(); d2_scheduled_timer.run();  //device 2's hardware switch timer and schedule timer

}
