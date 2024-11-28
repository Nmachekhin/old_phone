#include "Dial.h"

Dial::Dial(const short int dp)
{
  dial_pin=dp;
  pinMode(dial_pin, INPUT_PULLUP);
}

int Dial::get_reading()
{
  return digitalRead(dial_pin);
}


void Dial::operate_dial(unsigned long global_timer, SimModule& sim800l){
  if (!last_btn_state && get_reading())
  {
    last_btn_state = true;
    tick_timer = micros();
    dial_standby_timer=global_timer;
    sim800l.stop_tone();
  }
  if (last_btn_state && !get_reading())
  {
    last_btn_state = false;
    tick_arr[len]=micros()-tick_timer;
    len++;
  }
  if(len>1 && !last_btn_state && micros()-tick_timer>dial_btn_delay*1000)
  {
    current_number=0;
    for(int i=1; i<len-1; i++)
      if (tick_arr[i]>tick_arr[i-1] && tick_arr[i]>tick_arr[i+1] && tick_arr[i]>=11000)
        {
            //Serial.println(ar[i]);
            current_number++;
        }
    
    len=1;
    dialed_number+=current_number%10;
    //Serial.println(dialed_number);
  }
  
}

bool Dial::ready_to_call(unsigned long global_timer)
{
  return global_timer-dial_standby_timer>=dial_before_call_delay && dialed_number!="";
}


String Dial::get_number()
{
  return dialed_number;
}

void Dial::reset_number()
{
  dialed_number="";
}

void Dial::reset_timers(unsigned long global_timer)
{
    dial_timer=global_timer;
    dial_standby_timer=global_timer;
}
