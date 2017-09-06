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

int wifi_auth_error_count = 0;

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

//////////////////////////////////////////////////////
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

  WiFi.onEvent(WiFiEvent);

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

