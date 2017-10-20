#include <IRremote.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int debug_pin_1 = 13;                                                          //13
int ir_receiver_pin_1 = 2;                                                          //2
int hw_push_button_1_pin_1 = 4;                                                     //4
int hw_push_button_2_pin_1 = 7;                                                     //7
int device_1_output_pin_1 = 3;                                                      //3
int device_2_output_pin_1 = 5;                                                      //5
int device_1_manual_on_led_pin_1 = 6, device_1_manual_off_led_pin_1 = 9, device_1_automatic_on_led_pin_1 = 8;  //6-9-8
int device_2_manual_on_led_pin_1 = 10, device_2_manual_off_led_pin_1 = 11, device_2_automatic_on_led_pin_1 = 12; //10-11-12
int sda_pin_1 = 19, scl_pin_1 = 18;                                                 //19-18
int esp_tx_pin_1 = 15;                                                              //15(a1)
int esp_rx_pin_1 = 16;                                                              //16(a2)
int current_sensor_pin_1 = 14;                                                      //14(a0)
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
IRrecv irrecv(ir_receiver_pin_1);
decode_results results;
/////////////////////////////////////////////////////////////////////////////
unsigned long last = 0;

int device_1_output_state = LOW;
int device_2_output_state = LOW;

int hw_push_button_1_state;
int hw_push_button_1_last_State = LOW;
unsigned long hw_push_button_1_last_debounce_time = 0;
unsigned long hw_push_button_1_debounce_delay = 50;

int hw_push_button_2_state;
int hw_push_button_2_last_State = LOW;
unsigned long hw_push_button_2_last_debounce_time = 0;
unsigned long hw_push_button_2_debounce_delay = 50;

String ir_hex_received="";
String esp_serial_str="";
String esp_serial_string , esp_serial_char_first,esp_serial_char_rest;

/////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  pinMode(debug_pin_1, OUTPUT);
  pinMode(hw_push_button_1_pin_1, INPUT_PULLUP);
  pinMode(hw_push_button_2_pin_1, INPUT_PULLUP);
  pinMode(device_1_output_pin_1, OUTPUT);
  pinMode(device_2_output_pin_1, OUTPUT);
  pinMode(device_1_manual_on_led_pin_1, OUTPUT);
  pinMode(device_2_manual_on_led_pin_1, OUTPUT);
  pinMode(device_1_manual_off_led_pin_1, OUTPUT);
  pinMode(device_2_manual_off_led_pin_1, OUTPUT);
 // pinMode(device_1_automatic_on_led_pin_1, OUTPUT);
  //pinMode(device_2_automatic_on_led_pin_1, OUTPUT);
  irrecv.enableIRIn();
}

void loop() {
arduino_reset_func();
esp_config_init_func();
esp_serial_reader_func();
ir_receiver_func();
ir_decoder_func();
hw_push_button_1_func();
hw_push_button_2_func();
device_1_indicator_func();
device_2_indicator_func();
}
////////////////////////////////////////////////////////////////////////////
