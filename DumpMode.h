const int d1HardwarePin = 12; //d6 //lab
const int d2HardwarePin = 13; // d7

const int d1OpPin = 16; //0
const int d2OpPin = 14; //
bool status_init_flag=true;

/*for switching in dump mode*/

//int D1State = configStore.Dev_1_state; // initiating the state as low - device 1
//int D2State = configStore.Dev_2_state; // initiating the state as low - device 2
int D1State = LOW;
int D2State = LOW;

int D1HwSwitcState = !digitalRead(d1HardwarePin); // variable for holding current device switching state - device 1
int D2HwSwitcState = !digitalRead(d2HardwarePin); // variable for holding current device switching state - device 2


unsigned long D1_hw_switch_trig_millis = 0;
unsigned long D2_hw_switch_trig_millis = 0;

int con_mode_val = 0;
int con_mode_limit_val = 25;
unsigned long mode_changer_millis = 3000;

////////////////////////////////
////////////////////////////////
void switchTrigger(int dHardwarePin, int &DHwSwitcState, int &DState, int dOpPin, unsigned long &d_hw_switch_trig_millis, int D_id)
{
  if (digitalRead(dHardwarePin) != DHwSwitcState)
  {
    DHwSwitcState = (!DHwSwitcState);
    if ((millis() - d_hw_switch_trig_millis) < mode_changer_millis)
    {
      con_mode_val = con_mode_val + 1;
      Serial.println(con_mode_val);
    }
    else {
      con_mode_val = 0;
      Serial.println("zero");
    }
    d_hw_switch_trig_millis = millis();
  }
}
////////////////////////////////
void basicSwitchFunc() {
  switchTrigger(d1HardwarePin, D1HwSwitcState, D1State, d1OpPin, D1_hw_switch_trig_millis, 1);
  switchTrigger(d2HardwarePin, D2HwSwitcState, D2State, d2OpPin, D2_hw_switch_trig_millis, 2);
}
////////////////////////////////
void config_mode_chk_func() {
  if (con_mode_val > con_mode_limit_val) {
    BlynkState::set(MODE_WAIT_CONFIG);
  }
}
////////////////////////////////
void dev_status_init() {
  digitalWrite(d1OpPin, (!digitalRead(d1HardwarePin)));
  digitalWrite(d2OpPin, (!digitalRead(d2HardwarePin)));
}
////////////////////////////////
void enterDumbMode() {
  dev_status_init();
  basicSwitchFunc();
  config_mode_chk_func();
}

