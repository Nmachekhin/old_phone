#pragma once
#include "SimCommands.h"


class Dial{
 private:
  const unsigned long dial_btn_delay=250;
  const unsigned long dial_before_call_delay=4000; 
  bool last_btn_state=false;
  String dialed_number="";
  short current_number=0;
  short int dial_pin;
  unsigned long dial_timer;
  unsigned long dial_standby_timer;



  double tick_timer;
  double tick_arr[100]={0};
  int len=1;


  int get_reading();

 public:
  Dial()=default;
  Dial(const short int );
  void operate_dial(unsigned long, SimModule&);

  void reset_timers(unsigned long);
  bool ready_to_call(unsigned long);
  String get_number();
  void reset_number();

};