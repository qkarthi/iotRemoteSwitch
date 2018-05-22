#define APP_DEBUG
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>
#include "BlynkProvisioning.h"
#include <TimeLib.h>
#include <WidgetRTC.h>

WidgetRTC rtc; //declaring rtc for time getting from mobile
WidgetTerminal terminal(V99);  //declaring terminal
WidgetLED d1_led(V61), d2_led(V62); // declared led for device indcaation
//////////////////////////////////////////////////////
BlynkTimer d1_hw_switch_timer, d2_hw_switch_timer;
BlynkTimer conn_maint, upd_equipment_detail_timer;
BlynkTimer  scheduler_1_timer, scheduler_2_timer;
//////////////////////////////////////////////////////
String  client_id;
String TermString, TermCharOne, TermCharRest, TermTimeStr;
//////////////////////////////////////////////////////

int scheduler_1_var_1;
int scheduler_2_var_1;

int device_selected_scheduler_1;
int device_selected_scheduler_2;

long additional_scheduling_intervals = 9;
long scheduler_1_start_seconds;
long scheduler_2_start_seconds;

long scheduler_1_stop_seconds;
long scheduler_2_stop_seconds;
///////////////////////////////////////////
long nowseconds_var;
int dayadjustment_var;
int day_i;

/////////////////////////////////////////// !!!!!! Notes some variables are commented below ( because they are used as purpose in functionality under blynk_write function )

const int V_RssiPin = 2; // used for sending wifi signal to gauge

const int V_DStaStr = 5; // display the status

const int V_MasPowOnManButPin = 19;  // used for master power on below the device tiles
const int V_MasPowOffManButPin = 20; // used for master power off below the device tiles

const int V_D1ManButPin = 11;   // used as on off button for device 1
const int V_D2ManButPin = 12;   // used as on off button for device 2

//const int V_Sch1actButPin = 21;   // scheduer 1 activation button
//const int V_Sch2actButPin = 22;   // scheduer 2 activation button

const int V_D1Label = 31; //device 1 name pin
const int V_D2Label = 32; //device 2 name pin

//const int V_Sch1time_input = 51; //scheduler 1 time input pin
//const int V_Sch2time_input = 52; //scheduler 2 time input pin

//const int V_D1Led = 61; //device 1 Led pin
//const int V_D2Led = 62; //device 2 Led pin

const int V_DevSelector1 = 71; //device selector for scheduler 1 pin
const int V_DevSelector2 = 72; //device selector for scheduler 2 pin


//const int V_terminal = 99;  // terminal communication pin
//const int V_internal_otaButpin = 100;  // do not use this (100) because its only for sever based update method
//const int V_otaButpin = 111;  // it is used for user to update the device firm ware

const int V_colletive = 112;    // it is used for tile update .

///////////////////////////////////////////
bool FTC = true;
int d1_state;
int d1_hw_switch_1_state;

int d2_state;
int d2_hw_switch_1_state;
///////////////////////////////////////////
void setup() {

  client_id = String(WiFi.macAddress());

  delay(500);

  pinMode(d1_hw_switch_pin_1, INPUT_PULLUP); // declaring the pin as input-pullup
  pinMode(d2_hw_switch_pin_1, INPUT_PULLUP); // declaring the pin as input-pullup

  pinMode(d1_op_pin_1, OUTPUT); // declaring the pin as output
  pinMode(d2_op_pin_1, OUTPUT); // declaring the pin as output

  d1_hw_switch_1_state = digitalRead(d1_hw_switch_pin_1); // getting the state of hardware device 1 switch
  d2_hw_switch_1_state = digitalRead(d2_hw_switch_pin_1); // getting the state of hardware device 2 switch

  Serial.begin(115200);


  BlynkProvisioning.begin();


  d1_state =  configStore.Dev_1_state;
  d2_state =  configStore.Dev_2_state;

  digitalWrite(d1_op_pin_1, d1_state);
  digitalWrite(d2_op_pin_1, d2_state);

  if (d1_hw_switch_1_state != configStore.Dev_1_swi) {
    d1_state = !d1_state;
    d1_update_func(1, d1_hw_switch_1_state);
  }

  if (d2_hw_switch_1_state != configStore.Dev_2_swi) {
    d2_state = !d2_state;
    d2_update_func(1, d2_hw_switch_1_state);
  }

  d1_hw_switch_timer.setInterval(500, upd_d1_hw_switch_1_func);// setting the hardware switch timer with regular interval
  d2_hw_switch_timer.setInterval(500, upd_d2_hw_switch_1_func);// setting the hardware switch timer with regular interval


  scheduler_1_timer.setInterval(5000, scheduler_1_func); // setting the device 1 timer with regular interval
  scheduler_2_timer.setInterval(5000, scheduler_2_func); // setting the device 2 timer with regular interval

  conn_maint.setInterval(60000, con_maint_func);

  upd_equipment_detail_timer.setInterval(10000, upd_equipment_detail_func);

}

void loop() {

  BlynkProvisioning.run();

  if (configStore.flagConfig) {
    d1_hw_switch_timer.run();
    d2_hw_switch_timer.run();
    
    scheduler_1_timer.run();
    scheduler_2_timer.run();
    
    upd_equipment_detail_timer.run();
    
    conn_maint.run();
  }

  CoR_PollCounterFunc();
}
