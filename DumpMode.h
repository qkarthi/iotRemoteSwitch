/*for switching in dump mode*/

int D1HwSwitcState = !digitalRead(d1_hw_switch_pin_1); // variable for holding current device switching state - device 1

unsigned long D1_hw_switch_trig_millis = 0;

int con_mode_val = 0;
int con_mode_limit_val = 10;
unsigned long config_mode_changer_millis = 3000;

////////////////////////////////
void config_switchTrigger(int dHardwarePin, int &DHwSwitcState, unsigned long &d_hw_switch_trig_millis)
{
  if (digitalRead(dHardwarePin) != DHwSwitcState)
  {
    DHwSwitcState = (!DHwSwitcState);
    if (((millis() - d_hw_switch_trig_millis) < config_mode_changer_millis))
    {
      if ((millis() - d_hw_switch_trig_millis) > 50)
      {
        con_mode_val = con_mode_val + 1;
        Serial.println(con_mode_val);
      }
    }
    else {
      con_mode_val = 0;
    }
    d_hw_switch_trig_millis = millis();
  }
}
////////////////////////////////
void configPollCounterFunc() {
  config_switchTrigger(d1_hw_switch_pin_1, D1HwSwitcState, D1_hw_switch_trig_millis);
}
////////////////////////////////
void config_mode_chk_func() {
  if (con_mode_val >= con_mode_limit_val) {

    con_mode_val = 0;
    digitalWrite(d1_op_pin_1, LOW);
    digitalWrite(d2_op_pin_1, LOW);
    delay(1000);
    digitalWrite(d1_op_pin_1, HIGH);
    digitalWrite(d2_op_pin_1, HIGH);
    delay(1000);
    digitalWrite(d1_op_pin_1, LOW);
    digitalWrite(d2_op_pin_1, LOW);
    delay(1000);
    digitalWrite(d1_op_pin_1, HIGH);
    digitalWrite(d2_op_pin_1, HIGH);
    delay(1000);
    digitalWrite(d1_op_pin_1, LOW);
    digitalWrite(d2_op_pin_1, LOW);
    delay(1000);
    digitalWrite(d1_op_pin_1, HIGH);
    digitalWrite(d2_op_pin_1, HIGH);
    delay(1000);
    digitalWrite(d1_op_pin_1, LOW);
    digitalWrite(d2_op_pin_1, LOW);
    BlynkState::set(MODE_WAIT_CONFIG);
  }
}
////////////////////////////////
void dev_status() {
  digitalWrite(d1_op_pin_1, (!digitalRead(d1_hw_switch_pin_1)));
  digitalWrite(d2_op_pin_1, (!digitalRead(d2_hw_switch_pin_1)));
}
////////////////////////////////
void enterDumbMode() {
  dev_status();
  configPollCounterFunc();
  config_mode_chk_func();
}

