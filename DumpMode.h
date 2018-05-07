const int d1HardwarePin = 12; //d6 //lab
const int d2HardwarePin = 13; // d7

const int d1OpPin = 16; //0
const int d2OpPin = 14; //


/*for switching in dump mode*/

int D1State = LOW; // initiating the state as low - device 1
int D2State = LOW; // initiating the state as low - device 2

int D1HwSwitcState; // variable for holding current device switching state - device 1
int D2HwSwitcState; // variable for holding current device switching state - device 2


int D1ReadLogicState; // variable for reading logic change of device 1 in trigger
int D2ReadLogicState; // variable for reading logic change of device 2 in trigger

unsigned long D1_hw_switch_trig_millis = 0;
unsigned long D2_hw_switch_trig_millis = 0;

int con_mode_val = 0;
int con_mode_limit_val = 25;
unsigned long mode_changer_millis = 3000;

////////////////////////////////
void switchTrigger(int dHardwarePin, int &DHwSwitcState, int &DState, int dOpPin, unsigned long &d_hw_switch_trig_millis)
{
  if (digitalRead(dHardwarePin) != DHwSwitcState)
  {
    DHwSwitcState = (!DHwSwitcState);
    DState = (!DState) ;
    digitalWrite(dOpPin, DState);
    Serial.println(millis());
    Serial.println(d_hw_switch_trig_millis);
    Serial.println(mode_changer_millis);
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


  
  /*DEBUG_PRINT("Switch Status");
  digitalWrite(dHardwarePin, dOpPin);
  con_mode_val = con_mode_val + 1;
  Serial.println(con_mode_val);*/
}
////////////////////////////////
void basicSwitchFunc() {
  switchTrigger(d1HardwarePin, D1HwSwitcState, D1State,  d1OpPin, D1_hw_switch_trig_millis);
  switchTrigger(d2HardwarePin, D2HwSwitcState, D2State, d2OpPin, D2_hw_switch_trig_millis);
}
////////////////////////////////
void config_mode_chk_func() {
  if (con_mode_val > con_mode_limit_val) {
    BlynkState::set(MODE_WAIT_CONFIG);
  }
}
////////////////////////////////
void enterDumbMode() {
  basicSwitchFunc();
  config_mode_chk_func();
}

