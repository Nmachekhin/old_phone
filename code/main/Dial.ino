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
  
  if(global_timer-dial_timer<dial_btn_delay && get_reading()!=last_btn_state)
  {
    last_btn_state=get_reading();

    if (last_btn_state)
    {
      current_number++;
    }
    sim800l.stop_tone();
    dial_timer=global_timer;
  }else if (global_timer-dial_timer>=dial_btn_delay)
  {
    
    last_btn_state=get_reading();
    if(current_number>0 && dialed_number.length()<15)
    {
      dialed_number+=current_number%10;
      dial_standby_timer=global_timer;
    }
    current_number=0;
    dial_timer=global_timer;
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
