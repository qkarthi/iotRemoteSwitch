BLYNK_WRITE(V51) // time input widget for scheduling 1
{
  if (scheduler_1_var_1 == HIGH) //scd_button selected
  {
    TimeInputParam t(param);
    nowseconds_var = ((hour() * 3600) + (minute() * 60) + second());

    Serial.println("now  :" + String(nowseconds_var));
    dayadjustment_var = -1;
    if (weekday() == 1) // 1 as monday
    {
      dayadjustment_var =  6;
    }
    for (int day_i = 1; day_i <= 7; day_i++)
    {
      if (t.isWeekdaySelected(day_i))
      {
        if ((weekday() + dayadjustment_var) == day_i) // day selected
        {

          scheduler_1_start_seconds = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);
          Serial.println("start:" + String(scheduler_1_start_seconds));
          scheduler_1_stop_seconds = (t.getStopHour() * 3600) + (t.getStopMinute() * 60) + additional_scheduling_intervals;
          Serial.println("stop :" + String(scheduler_1_stop_seconds));
          if (((scheduler_1_start_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_1_start_seconds))
          {
            if (device_selected_scheduler_1 == 1)
            {
              d1_state = HIGH;
              d1_update_func(0, 0); //send values to devices for update
              Blynk.virtualWrite(V_D1ManButPin, d1_state);
            }
            if (device_selected_scheduler_1 == 2)
            {
              d2_state = HIGH;
              d2_update_func(0, 0); //send values to devices for update
              Blynk.virtualWrite(V_D2ManButPin, d2_state);
            }
          }
          /////////////////////////////////////////
          if ( ((scheduler_1_stop_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_1_stop_seconds))
          { if (device_selected_scheduler_1 == 1)
            {
              d1_state = LOW;
              d1_update_func(0, 0); //send values to devices for update
              Blynk.virtualWrite(V_D1ManButPin, d1_state);
            }
            if (device_selected_scheduler_1 == 2)
            {
              d2_state = LOW;
              d2_update_func(0, 0); //send values to devices for update
              Blynk.virtualWrite(V_D2ManButPin, d2_state);
            }
          }
        }
      }
    }
  }
}

BLYNK_WRITE(V52) // time input widget for scheduling 2
{
  if (scheduler_2_var_1 == HIGH) //scd_button selected
  {
    TimeInputParam t(param);
    nowseconds_var = ((hour() * 3600) + (minute() * 60) + second());
    dayadjustment_var = -1;
    if (weekday() == 1) // 1 as monday
    {
      dayadjustment_var =  6;
    }
    for (int day_i = 1; day_i <= 7; day_i++)
    {
      if (t.isWeekdaySelected(day_i))
      {
        if ((weekday() + dayadjustment_var) == day_i) // day selected
        {
          scheduler_2_start_seconds = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);
          scheduler_2_stop_seconds = (t.getStopHour() * 3600) + (t.getStopMinute() * 60) + additional_scheduling_intervals;
          if (((scheduler_2_start_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_2_start_seconds))
          { if (device_selected_scheduler_2 == 1)
            {
              d1_state = HIGH;
              d1_update_func(0, 0); //send values to devices for update
              Blynk.virtualWrite(V_D1ManButPin, d1_state);
            } if (device_selected_scheduler_2 == 2)
            {
              d2_state = HIGH;
              d2_update_func(0, 0); //send values to devices for update
              Blynk.virtualWrite(V_D2ManButPin, d2_state);
            }
          }
          /////////////////////////////////////////
          if ( ((scheduler_2_stop_seconds + additional_scheduling_intervals) >= nowseconds_var) & (nowseconds_var >= scheduler_2_stop_seconds))
          { if (device_selected_scheduler_2 == 1)
            {
              d1_state = LOW;
              d1_update_func(0, 0); //send values to devices for update
              Blynk.virtualWrite(V_D1ManButPin, d1_state);
            } if (device_selected_scheduler_2 == 2)
            {
              d2_state = LOW;
              d2_update_func(0, 0); //send values to devices for update
              Blynk.virtualWrite(V_D2ManButPin, d2_state);
            }
          }
        }
      }
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V21) // scheduler 1 activation button
{
  scheduler_1_var_1  = param.asInt();
}
BLYNK_WRITE(V22) // scheduler 2 activation button
{
  scheduler_2_var_1  = param.asInt();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
BLYNK_WRITE(V71) // scheduler 1 device selecter
{
  device_selected_scheduler_1 = param.asInt();
}
BLYNK_WRITE(V72) // scheduler 2 device selecter
{
  device_selected_scheduler_2 = param.asInt();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
