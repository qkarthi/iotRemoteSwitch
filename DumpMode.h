/*for switching in dump mode*/

int D1HwSwitcState = !digitalRead(d1_hw_switch_pin_1); // variable for holding current device switching state - device 1
int D2HwSwitcState = !digitalRead(d2_hw_switch_pin_1); // variable for holding current device switching state - device 1

unsigned long D1_hw_switch_trig_millis = 0;
unsigned long D2_hw_switch_trig_millis = 0;

int hw_switch_1_CoR_mode_val = 0;
int hw_switch_2_CoR_mode_val = 0;

int R_mode_limit_val = 10;
int C_mode_limit_val =  5;

unsigned long CoR_mode_changer_millis_inter = 3000;

////////////////////////////////////////////////////
void user_ind_blink() {
  digitalWrite(d1_op_pin_1, LOW);
  digitalWrite(d2_op_pin_1, LOW);
  delay(5000);
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
  delay(500);
}
/////////////////////////////////////////////////////
void CoR_mode_chk_func(int CoR_val_temp) {

  if (configStore.flagConfig) {

    if (CoR_val_temp >= R_mode_limit_val) {
      hw_switch_1_CoR_mode_val = 0;
      hw_switch_2_CoR_mode_val = 0;
      user_ind_blink();
      configStore = configDefault;
      config_save();

      delay(10);

      restartMCU();
      ///////////////////////////////////
    }
  } else {
    if (CoR_val_temp >= C_mode_limit_val) {
      hw_switch_1_CoR_mode_val = 0;
      hw_switch_2_CoR_mode_val = 0;
      user_ind_blink();
      BlynkState::set(MODE_WAIT_CONFIG);
    }
  }
}
////////////////////////////////
void CoR_switchTrigger(int dHardwarePin, int &DHwSwitcState, unsigned long &d_hw_switch_trig_millis, int &CoR_mode_val)
{
  if (digitalRead(dHardwarePin) != DHwSwitcState)
  {
    DHwSwitcState = (!DHwSwitcState);
    if (((millis() - d_hw_switch_trig_millis) < CoR_mode_changer_millis_inter))
    {
      if ((millis() - d_hw_switch_trig_millis) > 50)
      {
        CoR_mode_val = CoR_mode_val + 1;
        CoR_mode_chk_func(CoR_mode_val);
      }
    }
    else {
      CoR_mode_val = 0;
    }
    d_hw_switch_trig_millis = millis();
  }
}
////////////////////////////////
void CoR_PollCounterFunc() {
  CoR_switchTrigger(d1_hw_switch_pin_1, D1HwSwitcState, D1_hw_switch_trig_millis, hw_switch_1_CoR_mode_val );
  CoR_switchTrigger(d2_hw_switch_pin_1, D2HwSwitcState, D2_hw_switch_trig_millis, hw_switch_2_CoR_mode_val );
}

////////////////////////////////
void dev_status() {
  digitalWrite(d1_op_pin_1, (!digitalRead(d1_hw_switch_pin_1)));
  digitalWrite(d2_op_pin_1, (!digitalRead(d2_hw_switch_pin_1)));
}
////////////////////////////////
void enterDumbMode() {
  if (!configStore.flagConfig) {
    dev_status();
  }
  CoR_PollCounterFunc();
}
