/////////////////////////////////////////////////////////////////////////////
void(* resetFunc) (void) = 0;
/////////////////////////////////////////////////////////////////////////////
void dump(decode_results *results) {
  int count = results->rawlen;
  //Serial.println(results->value, HEX); /******************************/
  ir_hex_received = String(results->value, HEX);
}
/////////////////////////////////////////////////////////////////////////////
void esp_serial_reader_func() {
  if (Serial.available() > 0)
  {
    esp_serial_str = Serial.readString();
    //Serial.println(esp_serial_str);
    delay(5);
  }

  if (esp_serial_str != "") {
    esp_serial_char_first = esp_serial_str.charAt(0);
    if (esp_serial_char_first == "#") {
      device_switching_func();
    }
  }
  esp_serial_str = "";
}

void device_switching_func() {
  esp_serial_char_rest = esp_serial_str.substring(1, 6);
  if (esp_serial_char_rest == "d1onX")
  {
    device_1_output_state = HIGH;
    device_1_output_state_update_func();
  }
  if (esp_serial_char_rest == "d1off")
  {
    device_1_output_state = LOW;
    device_1_output_state_update_func();

  }
  if (esp_serial_char_rest == "d2onX")
  {
    device_2_output_state = HIGH;
    device_2_output_state_update_func();

  }
  if (esp_serial_char_rest == "d2off")
  {
    device_2_output_state = LOW;
    device_2_output_state_update_func();

  }
  if (esp_serial_char_rest == "aonXX")
  {
    device_1_output_state = HIGH;
    device_1_output_state_update_func();
    device_2_output_state = HIGH;
    device_2_output_state_update_func();
  }
  if (esp_serial_char_rest == "aoffX")
  {
    device_1_output_state = LOW;
    device_1_output_state_update_func();
    device_1_output_state = LOW;
    device_1_output_state_update_func();
  }
}
/////////////////////////////////////////////////////////////////////////////
void ir_receiver_func() {
  if (irrecv.decode(&results)) {
    if (millis() - last > 250) {
      dump(&results);
    }
    last = millis();
    irrecv.resume(); // Receive the next value
  }
}
//////////////////////////////////////////////////////////////////////////////
void arduino_reset_func() {
  if (millis() > 108000000) {
    resetFunc();
  }
}
void esp_config_init_func()
{

}
///////////////////////////////////////////////////////////////////////////////
void hw_push_button_1_func() {
  int hw_push_button_1_reading = digitalRead(hw_push_button_1_pin_1);
  if (hw_push_button_1_reading != hw_push_button_1_last_State) {
    hw_push_button_1_last_debounce_time = millis();
  }

  if ((millis() - hw_push_button_1_last_debounce_time) > hw_push_button_1_debounce_delay) {
    if (hw_push_button_1_reading != hw_push_button_1_state) {
      hw_push_button_1_state = hw_push_button_1_reading;
      if (hw_push_button_1_state == LOW) {
        device_1_output_state = !device_1_output_state;
        if (device_1_output_state)
        {
          Serial.println("#d1onX");
        }
        else {
          Serial.println("#d1off");
        }
        device_1_output_state_update_func();
      }
    }
  }
  hw_push_button_1_last_State = hw_push_button_1_reading;
}
///////////////////////////////////////////////////////////////////////////////
void hw_push_button_2_func() {
  int hw_push_button_2_reading = digitalRead(hw_push_button_2_pin_1);
  if (hw_push_button_2_reading != hw_push_button_2_last_State) {
    hw_push_button_2_last_debounce_time = millis();
  }

  if ((millis() - hw_push_button_2_last_debounce_time) > hw_push_button_2_debounce_delay) {
    if (hw_push_button_2_reading != hw_push_button_2_state) {
      hw_push_button_2_state = hw_push_button_2_reading;
      if (hw_push_button_2_state == LOW) {
        device_2_output_state = !device_2_output_state;
        if (device_2_output_state)
        {
          Serial.println("#d2onX");
        }
        else {
          Serial.println("#d2off");
        }
        device_2_output_state_update_func();
      }
    }
  }
  hw_push_button_2_last_State = hw_push_button_2_reading;
}
///////////////////////////////////////////////////////////////////////////////
void ir_decoder_func() {
  if (ir_hex_received == "fbf807")
  {
    device_1_output_state = HIGH;
    device_1_output_state_update_func();
    delay(500);
    device_2_output_state = HIGH;
    device_2_output_state_update_func();
    Serial.println("#aonXX");
  }
  if (ir_hex_received == "fb7887")
  {
    Serial.println("#aoffX");
    device_1_output_state = LOW;
    device_1_output_state_update_func();
    delay(500);
    device_2_output_state = LOW;
    device_2_output_state_update_func();
  }
  if (ir_hex_received == "fb807f")
  {
    device_1_output_state = !device_1_output_state;
    device_1_output_state_update_func();
    if (device_1_output_state)
    {
      Serial.println("#d1onX");
    }
    else {
      Serial.println("#d1off");
    }

  }
  if (ir_hex_received == "fb40bf")
  {
    device_2_output_state = !device_2_output_state;
    device_2_output_state_update_func();
    if (device_2_output_state)
    {
      Serial.println("#d2onX");
    }
    else {
      Serial.println("#d2off");
    }
  }


  ir_hex_received = "";
}
void device_1_output_state_update_func() {
  digitalWrite(device_1_output_pin_1, device_1_output_state);

}
void device_2_output_state_update_func() {
  digitalWrite(device_2_output_pin_1, device_2_output_state);

}

void device_1_indicator_func() {
  if (device_1_output_state == HIGH)
  {
    digitalWrite(device_1_manual_on_led_pin_1, HIGH);
    digitalWrite(device_1_manual_off_led_pin_1, LOW);
  } else {
    digitalWrite(device_1_manual_on_led_pin_1, LOW);
    digitalWrite(device_1_manual_off_led_pin_1, HIGH);
  }
}
void device_2_indicator_func() {
  if (device_2_output_state == HIGH)
  {
    digitalWrite(device_2_manual_on_led_pin_1, HIGH);
    digitalWrite(device_2_manual_off_led_pin_1, LOW);
  } else {
    digitalWrite(device_2_manual_on_led_pin_1, LOW);
    digitalWrite(device_2_manual_off_led_pin_1, HIGH);
  }
}
