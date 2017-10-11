#define APP_DEBUG
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include "BlynkProvisioning.h"
#include <TimeLib.h>
#include <WidgetRTC.h>
//////////////////////////////////////////////////////
BlynkTimer  con_manager, acs_timer, rtc_time_req_timer, upd_equipment_detail_timer, beep_timer;
BlynkTimer  d1_hw_switch_timer, d2_hw_switch_timer;
BlynkTimer  scheduler_1_timer, scheduler_2_timer;

WidgetTerminal terminal(V99);

WidgetLED d1_led(V61), d2_led(V62);

WidgetRTC rtc;

//////////////////////////////////////////////////////





//////////////////////////////////////////////////////
const int sensorIn = A0;

// buzzer = 4 - d2
// reset = 5 - d1

const int d1_op_pin_1 = 16; //0
const int d2_op_pin_1 = 14; //d5

//const int d1_hw_switch_pin_1 = 0; //d3 // nodemcu
const int d1_hw_switch_pin_1 = 12; //d6 //lab
const int d2_hw_switch_pin_1 = 13; // d7
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
const int vir_kwhr_reset_pin_1 = 9;
const int vir_kwhr_previous_pin_1 = 10;
///////////////////////////////////////////
const int vir_master_power_on_man_button_pin_1 = 19;
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
int beep_var = 0;
int disco_var = 0;
int sos_var = 0;
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
///////////////////////////////////////////
int mVperAmp = 75;
double sensor_result;
double VRMS = 0;
double AmpsRMS = 0;
float k_watt_hr_value;
int k_watt_hr_value_reset_maximum = 1;
/////////////////////////////////////////////
String term_string, term_char_one, term_char_rest, term_time_string, date_string;
/////////////////////////////////////////////
int d1_state = LOW;
int d1_hw_switch_1_state = HIGH;

int d2_state = LOW;
int d2_hw_switch_1_state = HIGH;
/////////////////////////////////////////////
void setup() {
  Serial.begin(115200); // declering the serial monitor with baudrate

  BlynkProvisioning.begin(); //blynk provision

  WiFi.onEvent(WiFiEvent);
  con_manager.setInterval(120000L, con_manager_func);

  rtc_time_req_timer.setInterval(60000L, requestTime_func); // setting the rtc timer with regular interval

  upd_equipment_detail_timer.setInterval(5000, upd_equipment_detail_func); // setting the equipment detail update timer with regular interval

  acs_timer.setInterval(3000, amp_mes_func); // setting the current sensor timer with regular interval

  beep_timer.setInterval(10000, beep_func); // setting the beep timer with regular interval

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
  beep_timer.run();

  d1_hw_switch_timer.run(); scheduler_1_timer.run();  //device 1's hardware switch timer and schedule timer
  d2_hw_switch_timer.run(); scheduler_2_timer.run();  //device 2's hardware switch timer and schedule timer
}
