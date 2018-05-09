/*for switching in fall back dump mode*/

int D2HwSwitcState = !digitalRead(d2_hw_switch_pin_1); // variable for holding current device switching state - device 1

unsigned long D2_hw_switch_trig_millis = 0;

int reset_mode_val = 0;
int reset_mode_limit_val = 15;
unsigned long reset_mode_changer_millis = 3000;
////////////////////////////////
void res_switchTrigger(int dHardwarePin, int &DHwSwitcState, unsigned long &d_hw_switch_trig_millis)
{
  if (digitalRead(dHardwarePin) != DHwSwitcState)
  {
    DHwSwitcState = (!DHwSwitcState);
    if (((millis() - d_hw_switch_trig_millis) < reset_mode_changer_millis))
    {
      if ((millis() - d_hw_switch_trig_millis) > 50)
      {
        reset_mode_val = reset_mode_val + 1;

        Serial.print("reset : ");
        Serial.println(reset_mode_val);
      }
    }
    else {
      reset_mode_val = 0;
    }
    d_hw_switch_trig_millis = millis();
  }
}
////////////////////////////////
void resetPollCounterFunc() {
  res_switchTrigger(d2_hw_switch_pin_1, D2HwSwitcState, D2_hw_switch_trig_millis);
}
////////////////////////////////
void reset_mode_chk_func() {
  if (reset_mode_val > reset_mode_limit_val) {
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
    ///////////////////////////////////

    configStore = configDefault;
    config_save();

    delay(10);

    restartMCU();
    ///////////////////////////////////
  }
}
////////////////////////////////
void FBDumbMode() {
  resetPollCounterFunc();
  reset_mode_chk_func();
}
