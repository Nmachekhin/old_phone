#pragma once
#include <SoftwareSerial.h>

enum SimStatus {
  READY=0,
  UNKNOWN=2,
  INCOMMING=3,
  INCALL=4
};

class SimModule{
private:
  unsigned long status_timer;
  const unsigned long status_delay=250;
  SoftwareSerial sim800l;
public:
  SimModule()=default;
  SimModule(uint8_t, uint8_t);

  void stop_tone();
  void play_tone(short, unsigned int);
  void hang();
  void accept();
  void dial(String);
  void check_status(unsigned long, SimStatus&);
};